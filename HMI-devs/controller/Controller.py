from PySide6.QtCore import QThreadPool
from controller.DataHandler import DataHandler
from controller.SVLoggerHandler import SVLoggerHandler
from controller.SensorsHandler import SensorsHandler
from typing import TYPE_CHECKING
from controller.SettingsHandler import SettingsHandler
from controller.SubscriberHandler import SubscriberHandler
from controller.LoggerHandler import LoggerHandler
from model.DataModel import DataModel
from model.MotorStatusData import MotorStatusData
from model.SealevelVsDepthData import SeaLevelVsDepthData
from model.TelemetryData import TelemetryData
from model.Timestamp import Timestamp
from persistence.SettingsReader import ConfigReader

if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class Controller:
    def __init__(self, view: "MainWindow", config_reader: ConfigReader):

        self.thread_pool = QThreadPool()
        self.thread_pool.setMaxThreadCount(10)
        print("Multithreading with maximum %d threads" % self.thread_pool.maxThreadCount())

        self.view = view

        self.__config = None

        self.telemetry_data = TelemetryData(cb=self.view.signals.request_graph_update.emit)
        self.sealevel_data = SeaLevelVsDepthData(cb=self.view.signals.request_histogram_update.emit)
        self.motor_status_data = MotorStatusData(cb=self.view.signals.request_motor_graph_update.emit)

        self.data_model = DataModel()

        self.telemetry_data.add_data("z", number_of_points=200)
        self.telemetry_data.add_data("psi")
        self.telemetry_data.add_data("roll")
        self.telemetry_data.add_data("pitch")
        self.telemetry_data.add_data("altitude")

        self.timestamp = Timestamp()

        self.config_reader = config_reader

        self.sensor_handler = SensorsHandler(view=self.view, timestamp=self.timestamp, thread_pool=self.thread_pool)

        self.logger_handler = LoggerHandler(view=self.view)

        self.sv_logger_handler = SVLoggerHandler(view=self.view, sensor_handler=self.sensor_handler)

        self.data_handler = DataHandler(
            view=self.view,
            sealevel_data=self.sealevel_data,
            telemetry_data=self.telemetry_data,
            motor_status_data=self.motor_status_data,
            data=self.data_model,
            timestamp=self.timestamp,
            sensor_handler=self.sensor_handler,
            logger_handler=self.logger_handler,
            thread_pool=self.thread_pool,
        )

        self.sensor_handler.set_client(self.data_handler.client)

        self.subscriber_handler = SubscriberHandler(view=self.view, thread_pool=self.thread_pool)

        self.settings_handler = SettingsHandler(view=self.view, update_settings_callback=self.save_and_update)

        self.__refresh_config()

        self.view.theme_switch.clicked.connect(self.switch_theme)

    def switch_theme(self):
        new_config = self.get_all_config()
        current_theme = self.get_config("color-theme")
        new_config["color-theme"] = "dark" if current_theme == "light" else "light"
        self.save_and_update(new_config)

    def save_and_update(self, new_configs):
        # Check if new config has all the same keys as the old one
        # in that case, add to new configs the old values
        for key, value in self.__config.items():
            if key not in new_configs:
                new_configs[key] = value

        self.config_reader.update(new_configs)
        self.__refresh_config()

    # Regresh config and update views paraeters
    def __refresh_config(self):
        self.__config = self.config_reader.read()

        # Update views
        self.logger_handler.update_parameters(
            logger_path=self.get_config("logger-main-path"),
            python_version=self.get_config("python-version"),
            lines=self.get_config("max-line-file"),
            topics_number=len(self.get_config("topics")),
        )

        self.sv_logger_handler.update_parameters(
            logger_path=self.get_config("svlogger-main-path"),
            python_version=self.get_config("python-version"),
        )

        self.data_handler.update_parameters(
            z_offset=self.get_config("default-z-offset"),
            broker_ip=self.get_config("broker-ip"),
            ground_ip=self.get_config("ground-machine-ip"),
        )

        self.data_model.altitude_model.set_thresholds(
            low_th=float(self.get_config("danger-threshold")),
            high_th=float(self.get_config("warning-threshold")),
        )

        self.subscriber_handler.update_parameters(
            broker_ip=self.get_config("broker-ip"),
            ground_ip=self.get_config("ground-machine-ip"),
            stop_script_path=self.get_config("subscriber-stop-script-path"),
            run_script_path=self.get_config("subscriber-run-script-path"),
        )

        self.view.switch_theme()

    def get_all_config(self):
        return self.__config

    def get_config(self, name):
        return self.__config.get(name)
