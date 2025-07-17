from PySide6.QtCore import QProcess
from PySide6.QtWidgets import QDialog, QFileDialog
from compiled_ui.ExportSVWindowUI import Ui_Form
from controller.Controller import Controller
from utils.theme_compiler import ThemeCompiler
from pathlib import Path

NO_FOLDER_MESSAGE = "No folder selected"
NO_FILE_MESSAGE = "No file selected"


class ExportSVDialog(Ui_Form, QDialog):
    def __init__(self, parent=None, controller: Controller = None):
        super().__init__(parent)
        self.setupUi(self)

        self.controller = controller

        self.source_file = None
        self.destination_folder = None

        self.export_source_profile_btn.clicked.connect(self.select_source_file)
        self.export_destination_folder_btn.clicked.connect(
            self.select_export_folder
        )
        self.export_profile_btn.clicked.connect(self.start_export)

        self.export_source_profile_file_label.dragEnterEvent = self.drag_event
        self.export_source_profile_file_label.dropEvent = self.drop_event_source

        self.export_destination_profile_folder_label.dragEnterEvent = (
            self.drag_event
        )
        self.export_destination_profile_folder_label.dropEvent = (
            self.drop_event_destination
        )

    def drag_event(self, e):
        if e.mimeData().hasUrls():
            e.accept()
        else:
            e.ignore()

    def drop_event_destination(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.destination_folder = f
            self.__update_view(
                source_file=self.source_file, destination_folder=f
            )

    def drop_event_source(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.source_file = f
            self.__update_view(
                source_file=f, destination_folder=self.destination_folder
            )

    def __select_folder(self):
        folderpath = QFileDialog.getExistingDirectory(self, "Select folder")
        if folderpath == "":
            folderpath = None
        return folderpath

    def __select_file(self):
        f = QFileDialog.getOpenFileName(self, "Select file")
        if f is not None:
            return f[0]
        return None

    def __update_view(
        self, source_file: str = None, destination_folder: str = None
    ):
        self.export_source_profile_file_label.setText(
            source_file if source_file is not None else NO_FILE_MESSAGE
        )
        self.export_destination_profile_folder_label.setText(
            destination_folder
            if destination_folder is not None
            else NO_FOLDER_MESSAGE
        )
        if source_file is not None and destination_folder is not None:
            self.export_profile_btn.setEnabled(True)
        else:
            self.export_profile_btn.setEnabled(False)

    def select_source_file(self):
        f = self.__select_file()
        if f is not None:
            self.source_file = f
            self.__update_view(
                source_file=f, destination_folder=self.destination_folder
            )

    def select_export_folder(self):
        f = self.__select_folder()
        if f is not None:
            self.destination_folder = f
            self.__update_view(
                destination_folder=f, source_file=self.source_file
            )

    def start_export(self):
        tare = self.export_tare.text()
        site_info = self.export_site_info.text()
        # Remove user extension from file name
        exported_file_name = (
            self.export_profile_file_name.text().split(".")[0] + ".txt"
        )

        latitude = self.controller.view.state_var_lat.text()
        # print(tare, site_info, exported_file_name, self.source_file, self.destination_folder, latitude)

        self.process = QProcess()

        self.process.stateChanged.connect(self.handle_state)
        self.process.readyReadStandardOutput.connect(self.read_stdout)
        self.process.readyReadStandardError.connect(self.handle_stderr)
        self.process.finished.connect(self.process_finished)

        # Args:
        #  0: exporter.py
        #  1: log_file path
        #  2: destination path
        #  3: site info
        #  4: tare
        #  5: latitude
        destination_path = str(
            Path(self.destination_folder).joinpath(exported_file_name)
        )

        print(
            self.controller.get_config("svexporter-path"),
            self.source_file,
            destination_path,
            site_info,
            tare,
            latitude,
        )

        self.process.start(
            self.controller.get_config("python-version"),
            [
                self.controller.get_config("svexporter-path"),
                self.source_file,
                destination_path,
                site_info,
                tare,
                latitude,
            ],
        )

        self.process.waitForStarted()

        self.export_profile_btn.setEnabled(False)

        print("start exporter")

    def show(self):
        super().show()

        existing_folder_path = self.controller.sv_logger_handler.folderpath

        if existing_folder_path is not None:
            self.source_file = str(
                Path(existing_folder_path).joinpath("sv_log.log")
            )
            if not Path(self.source_file).is_file():
                self.source_file = None
            self.__update_view(
                source_file=self.source_file,
                destination_folder=self.destination_folder,
            )

        theme_qss = ThemeCompiler.compile(
            self.controller.get_config("color-theme"), "exporter_style.txt"
        )
        self.setStyleSheet(theme_qss)
        self.exec()

    def stop(self):
        if self.process is not None:
            self.process.terminate()

    def process_finished(self):
        self.process.waitForFinished()
        self.export_profile_btn.setEnabled(True)
        print("stop exporter")

    def handle_state(self, state):
        states = {
            QProcess.NotRunning: "Not running",
            QProcess.Starting: "Starting",
            QProcess.Running: "Running",
        }
        state_name = states[state]
        print(f"State changed: {state_name}")

    def handle_stderr(self):
        data = self.process.readAllStandardError()
        stderr = bytes(data).decode("utf8")
        print(stderr)
        self.stop()

    def read_stdout(self):
        # self.message = self.process.readAllStandardOutput().data().decode('utf-8')
        data = self.process.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        print(stdout)
