from typing import TYPE_CHECKING
from PySide6.QtWidgets import QDoubleSpinBox, QLineEdit


if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class SettingsHandler:
    def __init__(self, view: "MainWindow", update_settings_callback):
        self.view = view
        self.update_settings_callback = update_settings_callback
        self.view.save_settings_btn.clicked.connect(self.__on_save)

        self.lines_edit = {
            "broker-ip": self.view.settings_broker_ip,
            "max-line-file": self.view.settings_max_lines,
            "logger-main-path": self.view.settings_logger_path,
            "python-version": self.view.settings_py_version,
            "exporter-path": self.view.settings_exporter_path,
            "default-export-format": self.view.settings_export_format,
            "ground-machine-ip": self.view.settings_ground_ip,
            "subscriber-stop-script-path": self.view.settings_sub_stop_path,
            "subscriber-run-script-path": self.view.settings_sub_run_path,
            "default-z-offset": self.view.settings_z_offset,
            "danger-threshold": self.view.settings_danger_threshold,
            "warning-threshold": self.view.settings_warning_threshold,
            "svlogger-main-path": self.view.settings_svlogger_path,
            "svexporter-path": self.view.settings_svexporter_path,
        }

        # self.view.settings_exporter_path.dropEvent = self.__drop_exporter_path
        # self.view.settings_logger_path.dropEvent = self.__drop_logger_path

        self.view.settings_logger_path.dropEvent = lambda e: self.__drop_path(e, self.view.settings_logger_path)
        self.view.settings_exporter_path.dropEvent = lambda e: self.__drop_path(e, self.view.settings_exporter_path)

        self.view.settings_svlogger_path.dropEvent = lambda e: self.__drop_path(e, self.view.settings_svlogger_path)
        self.view.settings_svexporter_path.dropEvent = lambda e: self.__drop_path(e, self.view.settings_svexporter_path)

    def __drop_path(self, event, component):
        f = [u.toLocalFile() for u in event.mimeData().urls()][0]
        component.setText(f)

    def fill_settings(self, settings):
        for key, value in settings.items():
            if key in self.lines_edit:
                if isinstance(self.lines_edit[key], QLineEdit):
                    self.lines_edit[key].setText(value)
                elif isinstance(self.lines_edit[key], QDoubleSpinBox):
                    self.lines_edit[key].setValue(float(value))

    def __on_save(self):
        settings = {}
        for key, value in self.lines_edit.items():
            if isinstance(value, QLineEdit):
                settings[key] = value.text()
            elif isinstance(value, QDoubleSpinBox):
                settings[key] = str(value.value())
            # text = value.text()
            # settings[key] = text

        self.update_settings_callback(settings)
