from typing import TYPE_CHECKING
from PySide6.QtCore import QThreadPool, Slot
import paho.mqtt.client as mqtt
from Enum.CommandStatus import BottomCamStatus, SensorStatus
from model.Sensor import Light, SciSensor, Sensor, NGCSensor
from model.BottomCam import BottomCam
from model.PilotCamRecorder import PilotCamRecorder
from model.Timestamp import Timestamp
from workers.BottomCamShutDownWorker import BottomCamShutdownWorker

if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class SensorsHandler:
    def __init__(self, view: "MainWindow", timestamp: Timestamp, thread_pool: QThreadPool) -> None:
        self.view = view
        self.client = None
        self.timestamp = timestamp

        self.__sensors: list[Sensor] = [
            Light("light_up", "ana_dig_io/DO11", topic_sub="ana_dig_io/DI11"),
            Light("light_md", "ana_dig_io/DO12", topic_sub="ana_dig_io/DI12"),
            Light("light_dw", "ana_dig_io/DO13", topic_sub="ana_dig_io/DI13"),
            SciSensor("miniCT", "ana_dig_io/DO09", topic_sub="ana_dig_io/DI09"),
            SciSensor("miniSVS", "ana_dig_io/DO10", topic_sub="ana_dig_io/DI10"),
            SciSensor("multi_beam", "ana_dig_io/DO25", topic_sub="ana_dig_io/DI25"),
            SciSensor("pilot_cam", "ana_dig_io/DO14", topic_sub="ana_dig_io/DI14"),
            NGCSensor("dvl", "ana_dig_io/DO08", topic_sub="ana_dig_io/DI08"),
            NGCSensor("usbl", "ana_dig_io/DO07", topic_sub="ana_dig_io/DI07"),
            NGCSensor("alt", "ana_dig_io/DO06", topic_sub="ana_dig_io/DI06"),
        ]

        self.threadpool = thread_pool

        self.bottom_cam = BottomCam()
        self.pilot_cam_record = PilotCamRecorder()

    def set_client(self, client: mqtt.Client):
        self.client = client

    def get_all_sensors(self):
        return self.__sensors

    def get_sensor_topics_sub(self):
        return [sensor.topic_sub for sensor in self.__sensors]

    def get_sensor_by_topic_sub(self, topic_sub: str) -> Sensor:
        for sensor in self.__sensors:
            if sensor.topic_sub == topic_sub:
                return sensor
        return None

    def get_sensor_by_name(self, name: str) -> Sensor:
        for sensor in self.__sensors:
            if sensor.name == name:
                return sensor
        return None

    def get_sensor_status(self, name: str) -> SensorStatus:
        sensor = self.get_sensor_by_name(name)
        if sensor is not None:
            return sensor.status
        raise Exception(f"Sensor {name} not found")

    def publish_status(self, name: str) -> Sensor:
        if self.client is None:
            raise Exception("Client not set")

        sensor = self.get_sensor_by_name(name)
        if sensor is not None:
            sensor.toggle_status()
            self.client.publish(sensor.topic_pub, f"{sensor.status.value} {int(self.timestamp.value)} 1")
            return sensor
        raise Exception(f"Sensor {name} not found")

    def handle_bottom_cam(self):
        if self.bottom_cam.status == BottomCamStatus.OFF:
            # Accensione
            self.client.publish(self.bottom_cam.cam_topic1, f"1 {int(self.timestamp.value)} 1")
            self.client.publish(self.bottom_cam.cam_topic2, f"1 {int(self.timestamp.value)} 1")
            self.client.publish(self.bottom_cam.net_topic, f"1 {int(self.timestamp.value)} 1")
        elif self.bottom_cam.status == BottomCamStatus.ON:
            # Spengo la net
            self.client.publish(self.bottom_cam.net_topic, f"0 {int(self.timestamp.value)} 1")
            self.bottom_cam.status = BottomCamStatus.SHUTTING_DOWN
            self.view.control_panel_handler.setStatus("bottom_cam", self.bottom_cam.status)
            

            # Avvio thread per spegnimento
            thread = BottomCamShutdownWorker()
            thread.signals.finished.connect(self.finished)
            thread.signals.progress.connect(self.report_progress)
            self.threadpool.start(thread)

    def handle_pilot_cam_record(self):
        if self.pilot_cam_record.status == SensorStatus.OFF:
            self.view.label_record.setText('recording')
            self.pilot_cam_record.turn_on()
            self.pilot_cam_record.status = SensorStatus.ON
        elif self.pilot_cam_record.status == SensorStatus.ON:
            self.view.label_record.setText('stopped')
            self.pilot_cam_record.turn_off()
            self.pilot_cam_record.status = SensorStatus.OFF
            

    @Slot(int)
    def report_progress(self, value):
        self.view.bottom_cam_timer_label.setText(f"Off in: {value}s")

    @Slot()
    def finished(self):
        # Spengo prima la cam
        self.client.publish(self.bottom_cam.cam_topic1, f"0 {int(self.timestamp.value)} 1")
        self.client.publish(self.bottom_cam.cam_topic2, f"0 {int(self.timestamp.value)} 1")
        self.bottom_cam.status = BottomCamStatus.OFF
        self.view.control_panel_handler.setStatus("bottom_cam", self.bottom_cam.status)
        self.view.bottom_cam_timer_label.setText("Bottom cam")
