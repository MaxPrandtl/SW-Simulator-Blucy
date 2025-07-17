from typing import TYPE_CHECKING
from PySide6.QtCore import QProcess
from PySide6.QtWidgets import QCheckBox, QFileDialog, QLabel, QPushButton, QSpinBox
from ViewHandlers.ViewStatusbarHandler import StatusBarHandler
from ViewHandlers.ViewStatusPageHandler import StatusPageHandler
from pathlib import Path
import datetime

if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class LoggerHandler:
    def __init__(self, view: "MainWindow") -> None:
        self.logger_path = None
        self.python_version = None
        self.lines = None

        self.view = view
        self.process = None

        self.__folderpath = None

        self.widget = view.main_content_widget.widget(1)

        self.status_bar_handler: StatusBarHandler = self.view.status_bar_handler
        self.status_page_handler: StatusPageHandler = self.view.status_page_handler

        self.view.selected_log_folder_path_label.dropEvent = self.__drop_log_folder

        self.__bind_events()

    def __drop_log_folder(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.__folderpath = f
            self.select_log_folder(open_dialog=False)

    @property
    def folderpath(self):
        return self.__folderpath

    def update_parameters(self, python_version, logger_path, lines, topics_number):
        self.python_version = python_version
        self.logger_path = logger_path
        self.lines = lines

        # Update UI
        self.view.max_line_selector.setValue(int(self.lines))

        topic_word = "topic" if topics_number == 1 else "topics"
        self.view.topic_selected_counter_label.setText(f"{topics_number} {topic_word} selected")

    def __bind_events(self):
        self.widget.findChild(QPushButton, "start_logger_btn").clicked.connect(self.start)
        self.widget.findChild(QPushButton, "stop_logger_btn").clicked.connect(self.stop)
        self.widget.findChild(QPushButton, "select_log_folder_btn").clicked.connect(
            lambda: self.select_log_folder(open_dialog=True)
        )
        self.widget.findChild(QPushButton, "select_topic_btn").clicked.connect(self.select_topic)
        self.widget.findChild(QPushButton, "export_btn").clicked.connect(self.export)

    def export(self):
        self.view.open_export_dialog()

    def select_topic(self):
        self.view.open_topic_selector_dialog()

    def select_log_folder(self, open_dialog=True):
        if open_dialog:
            self.__folderpath = QFileDialog.getExistingDirectory(self.view, "Select Log folder")

        if self.__folderpath == "":
            self.__folderpath = None
            self.widget.findChild(QPushButton, "start_logger_btn").setEnabled(False)
        else:
            self.widget.findChild(QPushButton, "start_logger_btn").setEnabled(True)
            now = datetime.datetime.now()
            log_folder_name = f'LOG_{now.strftime("%d-%m-%Y_%H-%M-%S")}'
            p = Path(self.__folderpath).joinpath(log_folder_name)
            p.mkdir()
            self.__folderpath = str(p)

        self.widget.findChild(QLabel, "selected_log_folder_path_label").setText(
            self.__folderpath if self.__folderpath is not None else "No folder selected"
        )

    def start(self):
        self.lines = self.widget.findChild(QSpinBox, "max_line_selector").value()

        if self.process is None:
            self.process = QProcess()

            self.process.stateChanged.connect(self.handle_state)
            self.process.readyReadStandardOutput.connect(self.read_stdout)
            self.process.readyReadStandardError.connect(self.handle_stderr)

            include_fog_data = "1" if self.widget.findChild(QCheckBox, "log_fog_data_checkbox").isChecked() else "0"
            # args:
            # 0: logger_path
            # 1: folder_path
            # 2: max_lines
            # 3: include_fog_data
            print('args', [
                    self.logger_path,
                    self.__folderpath,
                    str(self.lines),
                    0,
                ])
            self.process.start(
                self.python_version,
                [
                    self.logger_path,
                    self.__folderpath,
                    str(self.lines),
                    0,
                ],
            )

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
        print(f"[Main Logger]: {state_name}")

        if state == QProcess.Running:
            print(1)
            self.status_bar_handler.setStatus("logger", True)
            print(1)
            self.status_page_handler.setStatus("logger", True)
            print(1)
            self.widget.findChild(QPushButton, "start_logger_btn").setEnabled(False)
            print(1)
            self.widget.findChild(QPushButton, "stop_logger_btn").setEnabled(True)
            print(1)

        elif state == QProcess.NotRunning:
            self.status_bar_handler.setStatus("logger", False)
            self.status_page_handler.setStatus("logger", False)
            self.widget.findChild(QPushButton, "start_logger_btn").setEnabled(True)
            self.widget.findChild(QPushButton, "stop_logger_btn").setEnabled(False)
            self.process = None

    def handle_stderr(self):
        data = self.process.readAllStandardError()
        stderr = bytes(data).decode("utf8")
        print(stderr)
        self.stop()
        self.view.show_error_dialog(message=stderr, title="Logger error")

    def read_stdout(self):
        data = self.process.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        print(stdout)
