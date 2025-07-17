from typing import TYPE_CHECKING
from PySide6.QtCore import QProcess
from PySide6.QtWidgets import QFileDialog, QLabel, QMessageBox, QPushButton
from Enum.CommandStatus import SensorStatus
from controller.SensorsHandler import SensorsHandler
from ViewHandlers.ViewStatusbarHandler import StatusBarHandler
from ViewHandlers.ViewStatusPageHandler import StatusPageHandler
from pathlib import Path
import datetime


if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class SVLoggerHandler:
    def __init__(self, view: "MainWindow", sensor_handler: SensorsHandler) -> None:

        self.logger_path = None
        self.python_version = None

        self.view = view
        self.process = None

        self.__folderpath = None
        self.__profile_file_path = None

        self.widget = view.main_content_widget.widget(1)

        self.status_bar_handler: StatusBarHandler = self.view.status_bar_handler
        self.status_page_handler: StatusPageHandler = self.view.status_page_handler
        self.sensor_handler = sensor_handler

        self.view.selected_svlog_folder_path_label.dropEvent = self.__drop_log_folder
        self.view.selected_profile_path_label.dropEvent = self.__drop_profile_file_path

        self.__bind_events()

    def __drop_profile_file_path(self, event):
        f = [u.toLocalFile() for u in event.mimeData().urls()][0]
        if f is not None:
            self.__profile_file_path = f
            self.select_profile_file(open_dialog=False)

    def __drop_log_folder(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.__folderpath = f
            self.select_log_folder(open_dialog=False)

    def __bind_events(self):
        self.widget.findChild(QPushButton, "start_svlogger_btn").clicked.connect(self.start)
        self.widget.findChild(QPushButton, "stop_svlogger_btn").clicked.connect(self.stop)
        self.widget.findChild(QPushButton, "select_svlog_folder_btn").clicked.connect(
            lambda: self.select_log_folder(open_dialog=True)
        )
        self.widget.findChild(QPushButton, "export_sv_profile_btn").clicked.connect(self.export)
        self.widget.findChild(QPushButton, "select_profile_btn").clicked.connect(
            lambda: self.select_profile_file(open_dialog=True)
        )
        self.widget.findChild(QPushButton, "plot_profile_btn").clicked.connect(self.plot_profile)

    @property
    def folderpath(self):
        return self.__folderpath

    def update_parameters(self, python_version, logger_path):
        self.python_version = python_version
        self.logger_path = logger_path

    def plot_profile(self):
        data = self.__parse_profile_file()
        self.view.sv_plot_handler.plot(x=data[0], y=data[1])
        # self.view.sv_profile_plot.plot(x=data[0], y=data[1], title='Sound speed profile', pen='r')

    def __parse_profile_file(self):
        with open(self.__profile_file_path, "r") as f:
            lines = f.readlines()

        # Remove header
        lines = lines[9:]

        data = []
        for line in lines:
            tokens = line.split(" ")
            #              sound speed          depth
            data.append((float(tokens[2]), float(tokens[0])))

        # sort by pressure
        data = sorted(data, key=lambda x: x[1])

        return (list(map(lambda x: x[0], data)), list(map(lambda x: x[1], data)))

    def export(self):
        self.view.open_sv_export_dialog()

    def select_topic(self):
        self.view.open_topic_selector_dialog()

    def select_profile_file(self, open_dialog=True):
        if open_dialog:
            self.__profile_file_path = QFileDialog.getOpenFileName(self.view, "Select profile file", filter="*.txt")[0]

        if self.__profile_file_path == "":
            self.__profile_file_path = None
            self.widget.findChild(QPushButton, "plot_profile_btn").setEnabled(False)
        else:
            self.widget.findChild(QPushButton, "plot_profile_btn").setEnabled(True)
            self.widget.findChild(QLabel, "selected_profile_path_label").setText(
                self.__profile_file_path if self.__profile_file_path is not None else "No file selected"
            )

    def select_log_folder(self, open_dialog=True):
        if open_dialog:
            self.__folderpath = QFileDialog.getExistingDirectory(self.view, "Select Log folder")

        if self.__folderpath == "":
            self.__folderpath = None
            self.widget.findChild(QPushButton, "start_svlogger_btn").setEnabled(False)
        else:
            self.widget.findChild(QPushButton, "start_svlogger_btn").setEnabled(True)
            now = datetime.datetime.now()
            log_folder_name = f'SV_PROFILE_LOG_{now.strftime("%d-%m-%Y_%H-%M-%S")}'
            p = Path(self.__folderpath).joinpath(log_folder_name)
            p.mkdir()
            self.__folderpath = str(p)

        self.widget.findChild(QLabel, "selected_svlog_folder_path_label").setText(
            self.__folderpath if self.__folderpath is not None else "No folder selected"
        )

    def start(self):

        if self.process is None:
            ctd_sensors_status = []

            if self.sensor_handler.get_sensor_status("miniSVS") == SensorStatus.OFF:
                ctd_sensors_status.append("miniSVS")

            if self.sensor_handler.get_sensor_status("miniCT") == SensorStatus.OFF:
                ctd_sensors_status.append("miniCT")

            if len(ctd_sensors_status) > 0:
                ms = "\n".join(ctd_sensors_status) + "\n\n"
                message = f"The following sensors are not turned on: {ms}Please turn them on and try again."
                QMessageBox.warning(self.view, "Sensors off", message)
                return

            self.process = QProcess()

            self.process.stateChanged.connect(self.handle_state)
            self.process.readyReadStandardOutput.connect(self.read_stdout)
            self.process.readyReadStandardError.connect(self.handle_stderr)
            # self.process.finished.connect(self.process_finished)

            # args:
            # 0: logger_path
            # 1: folderpath
            self.process.start(self.python_version, [self.logger_path, self.__folderpath])

    def stop(self):
        if self.process is not None:
            self.process.close()

    def handle_state(self, state):
        states = {
            QProcess.NotRunning: "Not running",
            QProcess.Starting: "Starting",
            QProcess.Running: "Running",
        }

        state_name = states[state]
        print(f"[SVLogger]: {state_name}")

        if state == QProcess.Running:
            self.status_bar_handler.setStatus("svlogger", True)
            self.status_page_handler.setStatus("svlogger", True)
            self.widget.findChild(QPushButton, "start_svlogger_btn").setEnabled(False)
            self.widget.findChild(QPushButton, "stop_svlogger_btn").setEnabled(True)

        elif state == QProcess.NotRunning:
            self.status_bar_handler.setStatus("svlogger", False)
            self.status_page_handler.setStatus("svlogger", False)
            self.widget.findChild(QPushButton, "start_svlogger_btn").setEnabled(True)
            self.widget.findChild(QPushButton, "stop_svlogger_btn").setEnabled(False)
            self.process = None

    def handle_stderr(self):
        data = self.process.readAllStandardError()
        stderr = bytes(data).decode("utf8")
        print(stderr)
        self.stop()
        self.view.show_error_dialog(message=stderr, title="SVLogger error")

    def read_stdout(self):
        # self.message = self.process.readAllStandardOutput().data().decode('utf-8')
        data = self.process.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        print(stdout)
