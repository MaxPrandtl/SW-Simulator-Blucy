class StatusBarHandler:
    def __init__(self, view):
        self.view = view
        self.current_index = 0

        self.labels = {
            "logger": self.view.status_bar_logger,
            "svlogger": self.view.status_bar_svlogger,
            "mqtt": self.view.status_bar_mqtt,
            "ssh": self.view.status_bar_ssh,
            "sub": self.view.status_bar_subscriber,
        }

    def setStatus(self, name, value):
        # if value is boolean
        v = value
        if isinstance(value, bool):
            v = "on" if value else "off"

        self.labels[name].setProperty("status", v)
        self.labels[name].style().unpolish(self.labels[name])
        self.labels[name].style().polish(self.labels[name])
