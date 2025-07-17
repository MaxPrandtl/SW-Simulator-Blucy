from PySide6.QtCore import QProcess
from PySide6.QtWidgets import QDialog, QFileDialog
from compiled_ui.ExportWindowUI import Ui_export_window_main
from controller.Controller import Controller
from utils.theme_compiler import ThemeCompiler

NO_FOLDER_MESSAGE = "No folder selected"


class ExportDialog(Ui_export_window_main, QDialog):
    def __init__(self, parent=None, controller: Controller = None):
        super().__init__(parent)
        self.setupUi(self)

        self.controller = controller

        self.source_folder = None
        self.destination_folder = None

        self.source_btn.clicked.connect(self.select_source_folder)
        self.destination_btn.clicked.connect(self.select_export_folder)
        self.start_export_btn.clicked.connect(self.start_export)

        self.source_label.dragEnterEvent = self.drag_event
        self.source_label.dropEvent = self.drop_event_source

        self.destination_label.dragEnterEvent = self.drag_event
        self.destination_label.dropEvent = self.drop_event_destination

    def drag_event(self, e):
        if e.mimeData().hasUrls():
            e.accept()
        else:
            e.ignore()

    def drop_event_destination(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.destination_folder = f
            self.__update_view(source_folder=self.source_folder, destination_folder=f)

    def drop_event_source(self, e):
        f = [u.toLocalFile() for u in e.mimeData().urls()][0]
        if f is not None:
            self.source_folder = f
            self.__update_view(source_folder=f, destination_folder=self.destination_folder)

    def __select_folder(self, message: str = "Select folder"):
        folderpath = QFileDialog.getExistingDirectory(self, message)
        if folderpath == "":
            folderpath = None
        return folderpath

    def __update_view(self, source_folder: str = None, destination_folder: str = None):
        self.source_label.setText(source_folder if source_folder is not None else NO_FOLDER_MESSAGE)
        self.destination_label.setText(destination_folder if destination_folder is not None else NO_FOLDER_MESSAGE)
        if source_folder is not None and destination_folder is not None:
            self.start_export_btn.setEnabled(True)
        else:
            self.start_export_btn.setEnabled(False)

    def select_source_folder(self):
        f = self.__select_folder("Select logs folder")
        if f is not None:
            self.source_folder = f
            self.__update_view(source_folder=f, destination_folder=self.destination_folder)

    def select_export_folder(self):
        f = self.__select_folder("Select destination folder")
        if f is not None:
            self.destination_folder = f
            self.__update_view(destination_folder=f, source_folder=self.source_folder)

    def start_export(self):
        filters = self.filters_input.text()
        export_format = "csv" if self.csv_btn.isChecked() else "json"
        split_file = self.split_file_btn.isChecked()

        self.process = QProcess()

        self.process.stateChanged.connect(self.handle_state)
        self.process.readyReadStandardOutput.connect(self.read_stdout)
        self.process.readyReadStandardError.connect(self.handle_stderr)
        self.process.finished.connect(self.process_finished)
        # argv:
        # 0) exporter_path
        # 1) source_dir
        # 2) destination_dir
        # 3) filter_string
        # 4) format
        # 5) split_file option
        self.process.start(
            self.controller.get_config("python-version"),
            [
                self.controller.get_config("exporter-path"),
                self.source_folder,
                self.destination_folder,
                filters,
                export_format,
                str(split_file),
            ],
        )

        self.start_export_btn.setEnabled(False)
        # TODO: forse da mettere waitForFinished ??

        print("start exporter")

    def show(self):
        super().show()
        default_export = self.controller.get_config("default-export-format")

        self.__update_view(source_folder=self.controller.logger_handler.folderpath)
        self.source_folder = self.controller.logger_handler.folderpath

        if default_export == "json":
            self.json_btn.setChecked(True)
        else:
            self.csv_btn.setChecked(True)

        theme_qss = ThemeCompiler.compile(self.controller.get_config("color-theme"), "exporter_style.txt")
        self.setStyleSheet(theme_qss)
        self.exec()

    def stop(self):
        if self.process is not None:
            self.process.terminate()

    def process_finished(self):
        self.process.waitForFinished()
        self.start_export_btn.setEnabled(True)
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
        data = self.process.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        print(stdout)
