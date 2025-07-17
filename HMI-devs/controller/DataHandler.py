from typing import TYPE_CHECKING
from PySide6.QtCore import QThreadPool, Slot
from PySide6.QtWidgets import QPushButton
import paho.mqtt.client as mqtt
from Enum.CommandStatus import AltitudeStatus, BottomCamStatus, InfoLedStatus
from Signals import SignalCommunicate
from controller.LoggerHandler import LoggerHandler
from controller.SensorsHandler import SensorsHandler
from model.DataModel import DataModel
from model.MotorStatusData import MotorStatusData
from model.SealevelVsDepthData import SeaLevelVsDepthData
from model.TelemetryData import TelemetryData
from model.Timestamp import Timestamp
from utils.UDPSender import UDPSender
from workers.FogListenerWorker import UDPFogListenerWorker
from utils.DataProcess import Data

if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class DataHandler:
    def __init__(
        self,
        view: "MainWindow",
        timestamp: Timestamp,
        data: DataModel,
        telemetry_data: TelemetryData,
        sealevel_data: SeaLevelVsDepthData,
        motor_status_data: MotorStatusData,
        sensor_handler: SensorsHandler,
        logger_handler: LoggerHandler,
        thread_pool: QThreadPool,
    ) -> None:

        self.battery_cache = {}

        self.view = view
        self.sensor_handler = sensor_handler
        self.status_bar_handler = self.view.status_bar_handler
        self.status_page_handler = self.view.status_page_handler

        self.telemetry_data = telemetry_data
        self.sealevel_data = sealevel_data
        self.motor_status_data = motor_status_data

        self.data_model = data
        self.timestamp = timestamp

        # Traccia si il popup è stato mostrato
        self.water_leak_popup_shown = False

        self.logger_handler = logger_handler

        self.data_signal = SignalCommunicate()
        self.data_signal.update_data_variables.connect(self.update_data_variables)

        self.broker_ip = None
        self.logger_upd_sender = None
        self.z_offset = 0
        self.warning_threshold = 0
        self.danger_threshold = 0

        self.__sensors_topics = self.sensor_handler.get_sensor_topics_sub()

        # Mappa i topic con in componenti grafici che mostrano il valore
        self.map = {
            "NGC/force/tr/actual": self.view.data_tr,
            "NGC/force/fu/actual": self.view.data_fu,
            "NGC/force/fw/actual": self.view.data_fw,
            "NGC/force/fv/actual": self.view.data_fv,
            "NGC/velocity/body/r/actual": self.view.data_r,
            "NGC/velocity/body/u/actual": self.view.data_u,
            "NGC/velocity/body/v/actual": self.view.data_v,
            "NGC/velocity/body/w/actual": self.view.data_w,
            "NGC/pose/z/manual": self.view.state_var_ref_z,
            "NGC/pose/a/reference": self.view.state_var_ref_alt,  # TODO: Modificare da reference a manua
            "NGC/pose/psi/reference": self.view.state_var_ref_psi,
            "NGC/pose/z/actual": self.view.data_z,
            "ib_ins/heading": self.view.data_psi,
            "pa200/range": self.view.data_altitude,
            "miniSVS/pressure": self.view.data_pressure,
            "miniSVS/soundSpeed": self.view.data_sound_speed,
            "miniCT/temperature": self.view.data_temperature,
            "miniCT/conductivity": self.view.data_conductivity,
            "ana_dig_io/AI01": self.view.state_var_temperature,
        }

        self.unit_map = {
            "pa200/range": "m",
            "miniSVS/pressure": "bar",
            "miniSVS/soundSpeed": "m/s",
            "miniCT/temperature": "°C",
            "miniCT/conductivity": "S/cm",
            "ib_ins/heading": "°",
            "NGC/velocity/body/r/actual": "°/s",
            "NGC/pose/z/actual": "m",
        }

        self.state_map = {
            AltitudeStatus.SAFE: "safe",
            AltitudeStatus.WARNING: "warning",
            AltitudeStatus.DANGER: "danger",
        }

        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect
        self.client.on_connect_fail = self.on_connect_fail

        self.udp_fogger_worker = None
        self.udp_running = False

        self.fog_timer_worker = None
        self.fog_timer = False

        self.threadpool = thread_pool

        self.status_page_handler.setStatus("fog_udp", False)

    @Slot(mqtt.MQTTMessage)
    def update_data_variables(self, msg: mqtt.MQTTMessage):
        try:
            payload, ts, qos = msg.payload.decode("utf-8").strip().split(" ")
        except:
            payload, ts, qos, _ = msg.payload.decode("utf-8").strip().split(" ")
        # Gestione valore della batteria. Il valore letto da mqtt viene convertito in
        # percentuale da una funzione di interpolazione, che viene eseguita solamente
        # nel caso di nuovi dati.
        if msg.topic == "ana_dig_io/AI00":
            payload_value = msg.payload.decode("utf-8").strip().split(" ")[0]

            # Controllo se ho già elaborato il valore in modo da non eseguire
            # ogni volta l'interpolazione
            if payload_value in self.battery_cache:
                battery_value = self.battery_cache.get(payload_value)
            else:
                battery_value = int(Data.get_battery_fit(float(payload_value)) * 100)
                # Aggiungo alla cache il valore interpolato
                self.battery_cache[payload_value] = battery_value

            self.view.battery_progress_bar.setValue(battery_value)
            if battery_value>60:
                self.view.battery_progress_bar.setProperty('charge', 'high')
            elif battery_value> 30:
                self.view.battery_progress_bar.setProperty('charge', 'medium')
            else: 
                self.view.battery_progress_bar.setProperty('charge', 'low')
            self.view.battery_progress_bar.update()
            self.view.battery_progress_bar.updateGeometry()
            return

        # Rilevamento della modalità di pilotaggio (da completare)

        # -------------------- Gestione thrusters --------------------
        # Primo comando
        if msg.topic in self.data_model.thruster.keys():
            thruster = self.data_model.thruster.get(msg.topic)
            thruster.first_command = msg.payload.decode("utf-8").split(" ")[0] == "1"
            return

        # Secondo comando
        if msg.topic in self.data_model.get_thruster_enable_command_topic():
            thruster = self.data_model.get_thruster_by_enable_command(msg.topic)
            thruster.second_command = msg.payload.decode("utf-8").split(" ")[0] == "1"
            return

        # -------------------- Gestione messaggi relativi ai controlli --------------------
        if msg.topic in self.__sensors_topics:
            sensor_value = msg.payload.decode("utf-8").split(" ")[0]
            sensor = self.sensor_handler.get_sensor_by_topic_sub(msg.topic)
            if sensor is not None:
                sensor.status = sensor_value
                self.view.control_panel_handler.setStatus(sensor.name, sensor.status)
            return

        # Gestione dati mqtt per bottom cam
        if msg.topic in ["ana_dig_io/DI26", "ana_dig_io/DI24", "ana_dig_io/DI30"]:
            value = msg.payload.decode("utf-8").split(" ")[0]
            bottom_cam = self.sensor_handler.bottom_cam
            if value == "1":
                bottom_cam.status = BottomCamStatus.ON
                self.view.control_panel_handler.setStatus("bottom_cam", bottom_cam.status)
            return

        # Memorizzazione del timestamp
        if msg.topic == "clock/timestamp":
            self.timestamp.value = msg.payload.decode("utf-8").split(" ")[0]
            return

        if msg.topic == "ana_dig_io/DI22":
            msg_value = msg.payload.decode("utf-8").split(" ")[0]
            value: InfoLedStatus = InfoLedStatus.NO_COLOR if msg_value == "1" else InfoLedStatus.OFF
            self.view.control_panel_handler.set_info_led_status("water_leak", value)

            if value == InfoLedStatus.OFF:
                if self.water_leak_popup_shown is False:
                    self.view.show_error_dialog(title="Water leakage", message="Water leakage detected")
                    self.water_leak_popup_shown = True
            return

        # ----------- Memorizzo i dati nel modello ------------
        self.data_model.update(msg)

        # Gestione dati pitch e roll: aggiornamento grafici, le variabili nella barra
        # di stato vengono aggiornate automaticamente dalle funzioni update_roll e updata_pitch
        if msg.topic == "ib_ins/roll":
            self.telemetry_data.update_data("roll", self.data_model.ahrs.roll)
            self.view.data_page_controls_handler.update_roll()
            return
        elif msg.topic == "ib_ins/pitch":
            self.telemetry_data.update_data("pitch", self.data_model.ahrs.pitch)
            self.view.data_page_controls_handler.update_pitch()
            return

        # Gestione di dati che necessitano di una elaborazione prima di essere mostrati
        # oppure che devono essere plottati in telemetria
        elif msg.topic == "NGC/pose/z/actual":
            self.sealevel_data.z = self.data_model.z_model.z
            self.telemetry_data.update_data("z", self.data_model.z_model.z)

        elif msg.topic == "ib_ins/heading":
            self.view.data_page_controls_handler.update_compass()
            self.view.data_psi.setText(str(self.data_model.ahrs.heading))
            # self.telemetry_data.update_data('psi', v['timestamp'], v['value'])

        elif msg.topic == "pa200/range":
            self.sealevel_data.altitude = self.data_model.altitude_model.altitude
            self.telemetry_data.update_data("altitude", self.data_model.altitude_model.altitude)
            self.__apply_altitude_color_code()

        elif msg.topic == "NGC/pose/psi/reference":
            self.view.data_page_controls_handler.update_compass_reference()

        # TODO: aggiungere gli altri manual
        elif msg.topic == "NGC/pose/z/manual":
            self.sealevel_data.z_reference = self.data_model.reference.z

        # Gestione motor status
        elif msg.topic == "NGC/force/fu/actual":
            self.motor_status_data.update_data("surge", self.data_model.fu)

        elif msg.topic == "NGC/force/fv/actual":
            self.motor_status_data.update_data("sway", self.data_model.fv)

        elif msg.topic == "NGC/force/fw/actual":
            self.motor_status_data.update_data("heave", self.data_model.fw)

        elif msg.topic == "NGC/force/tr/actual":
            self.motor_status_data.update_data("yaw", self.data_model.tr)

        elif msg.topic in ["us_imu/latitude", "us_imu/longitude"]: 
            if qos == '1': 
                self.view.control_panel_handler.set_info_led_status("ahrs_status", InfoLedStatus.ON)
                from workers.FogActivator import FogActivator

                if self.fog_timer is False:
                    self.fog_timer_worker = FogActivator()
                    self.fog_timer_worker.signals.finished.connect(self.reset_udp_running)
                    self.fog_timer_worker.signals.error.connect(self.reset_udp_running)
                    self.fog_timer_worker.signals.on_data.connect(self.set_xy_data)
                    self.fog_timer_worker.signals.startup_completed.connect(self.__set_fog_led_status)
                    self.fog_timer_worker.signals.progress.connect(self.__show_fog_process)
                    self.threadpool.start(self.fog_timer_worker)
                    self.fog_timer = True
            if qos == '0': 
                self.view.control_panel_handler.set_info_led_status("ahrs_status", InfoLedStatus.WARN)
           
            


        # Aggiornamento delle caselle di testo nella sezione view Data
        data_string = str(str(self.data_model.get_data(msg.topic)))
        if msg.topic in self.unit_map:
            data_string += " " + self.unit_map[msg.topic]
        try:
            self.map.get(msg.topic).setText(data_string)
        except:
            pass

    # ------------ CONNECTIONS ------------
    def on_connect_fail(self):
        print("Errore di connessione mqtt client")
        self.status_bar_handler.setStatus("mqtt", "off")
        self.status_page_handler.setStatus("mqtt", "off")

    def on_disconnect(self, client, userdata, rc):
        print("mqtt client disconnesso")
        self.reconnect()

    # Riconnessione client mqtt
    def reconnect(self):
        self.status_bar_handler.setStatus("mqtt", False)
        self.status_page_handler.setStatus("mqtt", False)
        try:
            if self.broker_ip is None:
                return
            else:
                print("reconnecting mqtt")
                self.enable_all_controls(False)
                self.status_bar_handler.setStatus("mqtt", "starting")
                self.status_page_handler.setStatus("mqtt", "starting")
                self.client.loop_stop()
                self.client.disconnect()
                self.client.connect_async(host=self.broker_ip, port=1883, keepalive=60, clean_start=True)
                self.client.loop_start()
        except Exception as e:
            print("Errore di connessione client data")
            print(e)

    # Connessione client mqtt
    def on_connect(self, client, userdata, flags, rc):
        print("mqtt client connesso")
        self.enable_all_controls(True)
        topics = []
        # Iscrizione a topic aggiuntivi
        topics.append(("clock/timestamp", 1))
        topics.append(("ib_ins/roll", 1))
        topics.append(("ib_ins/pitch", 1))

        # Water leak detection
        topics.append(("ana_dig_io/DI22", 1))

        # Bottom cam (net e cam)
        topics.append(("ana_dig_io/DI30", 1))
        topics.append(("ana_dig_io/DI24", 1))
        topics.append(("ana_dig_io/DI26", 1))

        # Thrusters
        topics.append(("ana_dig_io/DI00", 1))
        topics.append(("thrusters/EnableCommand1", 1))
        topics.append(("ana_dig_io/DI01", 1))
        topics.append(("thrusters/EnableCommand2", 1))
        topics.append(("ana_dig_io/DI02", 1))
        topics.append(("thrusters/EnableCommand3", 1))
        topics.append(("ana_dig_io/DI03", 1))
        topics.append(("thrusters/EnableCommand4", 1))
        topics.append(("ana_dig_io/DI04", 1))
        topics.append(("thrusters/EnableCommand5", 1))
        topics.append(("ana_dig_io/DI05", 1))
        topics.append(("thrusters/EnableCommand6", 1))

        # FOG
        topics.append(("us_imu/latitude", 1))
        topics.append(("us_imu/longitude", 1))

        # Batteria
        topics.append(("ana_dig_io/AI00", 1))

        # Iscrizione a topic relativi ai controlli
        for sensor_topic in self.__sensors_topics:
            topics.append((sensor_topic, 1))

        for topic in self.map.keys():
            topics.append((topic, 1))
        client.subscribe(topics)

        self.status_bar_handler.setStatus("mqtt", True)
        self.status_page_handler.setStatus("mqtt", True)

    # Arrivo di un messaggio mqtt
    def on_message(self, client, userdata, msg):
        self.data_signal.update_data_variables.emit(msg)

    # ----------------------------------

    # Restart connessioni e parametri
    def update_parameters(self, z_offset=0, broker_ip=None, ground_ip=None):
        # self.z_offset = float(z_offset)
        # self.__add_z_offset(override=True)

        self.data_model.set_z_offset(float(z_offset))

        reconnect = self.broker_ip != broker_ip
        self.broker_ip = broker_ip

        if reconnect:
            self.reconnect()

        if self.udp_running is False:
            self.udp_fogger_worker = UDPFogListenerWorker(view=self.view, ground_ip=ground_ip)
            self.udp_fogger_worker.signals.finished.connect(self.reset_udp_running)
            self.udp_fogger_worker.signals.error.connect(self.reset_udp_running)
            self.udp_fogger_worker.signals.on_data.connect(self.set_xy_data)
            self.udp_fogger_worker.signals.startup_completed.connect(self.__set_fog_led_status)
            self.udp_fogger_worker.signals.progress.connect(self.__show_fog_process)
            self.threadpool.start(self.udp_fogger_worker)
            self.status_page_handler.setStatus("fog_udp", True)
            self.udp_running = True

        self.logger_upd_sender = UDPSender(host=ground_ip, port=2222)

    @Slot(int)
    def __show_fog_process(self, time):
        print(f"Fog on in {time}")
        self.view.label_7.setText(f"{time}s")

    @Slot()
    def __set_fog_led_status(self):
        self.view.label_7.setText("FOG")
        self.view.control_panel_handler.set_info_led_status("fog_status", InfoLedStatus.ON)

    # Setto i dati lon e lat nelle variabili globali
    # TODO: per il momento le invio al logger tramite UDP. Togliere se
    #       si riesce a fare multicasting
    @Slot(tuple)
    def set_xy_data(self, data):
        self.data_model.position.lat = data[0]
        self.data_model.position.lon = data[1]

        self.view.data_x.setText(data[1])
        self.view.data_y.setText(data[0])

        # Send data to logger via udp
        if self.logger_handler.process is not None:
            # format: x&y
            self.logger_upd_sender.send(f"{data[1]}&{data[0]}")

        self.view.state_var_lat.setText(data[0])
        self.view.state_var_lon.setText(data[1])

    @Slot()
    def reset_udp_running(self):
        self.status_page_handler.setStatus("fog_udp", False)
        self.udp_running = False

    # Funzioni per l'elaborazione di dati
    def __apply_altitude_color_code(self):
        self.view.data_altitude.setProperty("status", self.state_map[self.data_model.altitude_model.state])
        self.view.data_altitude.style().unpolish(self.view.data_altitude)
        self.view.data_altitude.style().polish(self.view.data_altitude)

    def enable_all_controls(self, val):
        btn: QPushButton
        for btn in self.view.controls_container.findChildren(QPushButton):
            prop = btn.dynamicPropertyNames()
            if "control" in prop:
                btn.setEnabled(val)
