from typing import TYPE_CHECKING
import time
import os
from PySide6.QtCore import QThreadPool, Slot
from Enum.CommandStatus import InfoLedStatus
from Enum.SubscriberSensorStatus import SubscriberSensorStatus
from ViewHandlers.ViewStatusbarHandler import StatusBarHandler
from ViewHandlers.ViewStatusPageHandler import StatusPageHandler
from paramiko import SSHClient, AutoAddPolicy
from workers.SubscriberDataTimerWorker import SubscriberDataTimerWorker

from workers.SubscriberListenerWorker import UDPSubscriberListenerWorker

TIME_SENSOR_OFF_NS = 2000000000

if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class SubscriberHandler:

    SSH_TIMEOUT = 5

    def __init__(self, view: "MainWindow", thread_pool: QThreadPool) -> None:
        self.view = view
        self.status_bar_handler: StatusBarHandler = self.view.status_bar_handler
        self.status_page_handler: StatusPageHandler = self.view.status_page_handler

        self.SSH_PASSWORD = os.getenv("SSH_PASSWORD")
        self.SSH_USERNAME = os.getenv("SSH_USERNAME")

        self.subscriber_process = None
        self.subscriber_process_pid = None

        self.ssh_connected = False

        self.running = False

        self.ssh_client = SSHClient()
        self.ssh_client.set_missing_host_key_policy(AutoAddPolicy())

        self.view.start_sub_btn.clicked.connect(self.create_process)
        self.view.stop_sub_btn.clicked.connect(self.stop_process)
        self.view.reconnect_ssh_btn.clicked.connect(self.connect_ssh_client)
        self.view.stop_sub_btn.setEnabled(False)

        self.broker_ip = None
        self.ground_ip = None
        self.run_script_path = None
        self.stop_script_path = None

        self.udp_subscriber_worker = None
        self.subscriber_data_timer_worker = None
        self.time_running = False
        self.udp_running = False
        self.threadpool = thread_pool

        self.miniSVS_timestamp = 0
        self.us_imu_timestamp = 0
        self.now = 0

    def update_parameters(self, broker_ip, ground_ip, run_script_path, stop_script_path):
        reconnect = self.broker_ip != broker_ip

        self.broker_ip = broker_ip

        self.ground_ip = ground_ip
        self.run_script_path = run_script_path
        self.stop_script_path = stop_script_path

        if reconnect:
            self.connect_ssh_client()

    def reset_udp_running(self):
        self.udp_running = False

    @Slot(int)
    def handle_udp_error(self, error):
        self.udp_running = False
        if error == 1:
            self.view.show_error_dialog(
                message="Errore durante la connessione UDP",
                title="Connessione UDP col subscriber fallita",
            )
        elif error == 2:
            self.view.show_error_dialog(
                message="Errore durante la ricezione dati UDP",
                title="Errore durante la comunicazione col subscriber",
            )

    @Slot(tuple)
    def set_sub_data_status(self, data):
        if data[0] == "miniSVS":
            self.miniSVS_timestamp = time.time_ns()
        else:
            self.us_imu_timestamp = time.time_ns()
            # Setto il colore della spia AHRS
            if data[1] == SubscriberSensorStatus.OK:
                self.view.control_panel_handler.set_info_led_status("ahrs_status", InfoLedStatus.ON)
            else:
                self.view.control_panel_handler.set_info_led_status("ahrs_status", InfoLedStatus.WARN)
        self.status_page_handler.set_subscriber_data_status(data[0], data[1])

    def connect_ssh_client(self):
        self.status_bar_handler.setStatus("ssh", False)
        self.status_page_handler.setStatus("ssh", False)

        try:
            print("Connessione ssh: " + self.broker_ip + " ...")
            self.status_bar_handler.setStatus("ssh", "starting")

            self.ssh_client.connect(
                self.broker_ip,
                username=self.SSH_USERNAME,
                password=self.SSH_PASSWORD,
                timeout=self.SSH_TIMEOUT,
            )
            self.ssh_connected = True
            self.status_bar_handler.setStatus("ssh", True)
            self.status_page_handler.setStatus("ssh", True)

            self.view.ssh_error_alert_frame.setVisible(False)
            self.view.sub_button_frame.setVisible(True)
        except Exception as e:
            self.ssh_connected = False
            self.status_bar_handler.setStatus("ssh", False)
            self.status_page_handler.setStatus("ssh", False)

            self.view.sub_button_frame.setVisible(False)
            self.view.ssh_error_alert_frame.setVisible(True)
            print("Errore di connessione ssh:", e)

    def create_process(self):
        if self.ssh_connected is False:
            raise ConnectionError("Errore di connessione ssh")

        self.subscriber_process = self.ssh_client.exec_command(f"bash -s < {self.run_script_path} {self.ground_ip}")

        # Lettura del pid del processo da std_output
        self.subscriber_process_pid = self.subscriber_process[1].readline()
        if len(self.subscriber_process_pid) == 0:
            self.view.show_error_dialog(
                message="Errore durante l'avvio del subscriber",
                title="Subscriber error",
            )
            return

        print("Subscriber running")
        self.status_bar_handler.setStatus("sub", True)
        self.status_page_handler.setStatus("sub", True)
        self.running = True

        self.view.start_sub_btn.setEnabled(False)
        self.view.stop_sub_btn.setEnabled(True)

        if self.udp_running is False:
            self.udp_subscriber_worker = UDPSubscriberListenerWorker(view=self.view, ground_ip=self.ground_ip)
            self.udp_subscriber_worker.signals.finished.connect(self.reset_udp_running)
            self.udp_subscriber_worker.signals.error.connect(self.handle_udp_error)
            self.udp_subscriber_worker.signals.on_data.connect(self.set_sub_data_status)
            self.threadpool.start(self.udp_subscriber_worker)
            self.udp_running = True

            self.subscriber_data_timer_worker = SubscriberDataTimerWorker(
                miniSVS_timestamp=self.miniSVS_timestamp,
                us_imu_timestamp=self.us_imu_timestamp,
            )
            self.subscriber_data_timer_worker.signals.time_up.connect(self.handle_sensor_off)
            self.threadpool.start(self.subscriber_data_timer_worker)
            self.time_running = True

    def handle_sensor_off(self, event):
        self.now = time.time_ns()
        if self.now - self.miniSVS_timestamp > TIME_SENSOR_OFF_NS:
            self.status_page_handler.set_subscriber_data_status("miniSVS", SubscriberSensorStatus.OFF)
        if self.now - self.us_imu_timestamp > TIME_SENSOR_OFF_NS:
            self.status_page_handler.set_subscriber_data_status("us_imu", SubscriberSensorStatus.OFF)

    def stop_process(self):
        if self.ssh_connected is False:
            print("Stop: Errore di connessione ssh")

        self.running = False
        if self.subscriber_process is not None:
            self.ssh_client.exec_command(f"bash -s < {self.stop_script_path} {self.subscriber_process_pid}")

            self.status_bar_handler.setStatus("sub", False)
            self.status_page_handler.setStatus("sub", False)

            self.view.start_sub_btn.setEnabled(True)
            self.view.stop_sub_btn.setEnabled(False)

            print("Subscriber stopped")
            self.subscriber_process = None

            self.udp_subscriber_worker.stop()
            self.subscriber_data_timer_worker.stop()

            # Spengo spia AHRS
            self.view.control_panel_handler.set_info_led_status("ahrs_status", InfoLedStatus.WARN)

            self.status_page_handler.set_subscriber_data_status("miniSVS", SubscriberSensorStatus.UPD_DISCONNECTED)
            self.status_page_handler.set_subscriber_data_status("us_imu", SubscriberSensorStatus.UPD_DISCONNECTED)
