from Enum.SubscriberSensorStatus import SubscriberSensorStatus


class StatusPageHandler:
    def __init__(self, view):
        self.view = view

        self.labels = {
            "logger": self.view.status_page_logger,
            "svlogger": self.view.status_page_svlogger,
            "ssh": self.view.status_page_ssh,
            "sub": self.view.status_page_sub,
            "mqtt": self.view.status_page_mqtt,
            "fog_udp": self.view.status_page_fog_udp,
            "miniSVS": self.view.status_page_minisvs,
            "us_imu": self.view.status_page_us_imu,
        }

        self.COLORS = {
            "on": "green",
            "off": "red",
            "starting": "orange",
        }

        self.SENSOR_PROPERTY_MAP = {
            SubscriberSensorStatus.OK: "ok",
            SubscriberSensorStatus.OFF: "off",
            SubscriberSensorStatus.NOT_VALID: "invalid",
            SubscriberSensorStatus.TIMESTAMP_ERROR: "timestamp",
            SubscriberSensorStatus.UPD_DISCONNECTED: "disconnected",
        }

        self.set_subscriber_data_status("miniSVS", SubscriberSensorStatus.UPD_DISCONNECTED)
        self.set_subscriber_data_status("us_imu", SubscriberSensorStatus.UPD_DISCONNECTED)

    def set_subscriber_data_status(self, name, value: SubscriberSensorStatus):
        if value is None:
            return
        self.labels[name].setProperty("sub_data_status", self.SENSOR_PROPERTY_MAP[value])
        self.labels[name].style().unpolish(self.labels[name])
        self.labels[name].style().polish(self.labels[name])

    def setStatus(self, name, value):
        main_text = self.labels[name].text().split(":")[0]
        self.labels[name].setText(self.__gen_html(main_text, value))

    def __gen_html(self, main_text, value):
        if isinstance(value, str):
            value_str = value.upper()
            color = self.COLORS[value]

        elif isinstance(value, bool):
            color = "green" if value else "red"
            value_str = "ON" if value else "OFF"

        return f'<html>{main_text}: <span style="color:{color}">{value_str}</span></html>'
