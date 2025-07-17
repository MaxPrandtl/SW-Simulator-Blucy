from Enum.CommandStatus import SensorStatus


class Sensor:
    def __init__(self, name, topic_pub, topic_sub) -> None:
        self.__name = name
        self.__topic_pub = topic_pub
        self.__topic_sub = topic_sub
        self.__status = SensorStatus.OFF

    @property
    def name(self):
        return self.__name

    @property
    def topic_pub(self) -> str:
        return self.__topic_pub

    @property
    def topic_sub(self) -> str:
        return self.__topic_sub

    @property
    def status(self) -> SensorStatus:
        return self.__status

    @status.setter
    def status(self, value: str):
        self.__status = SensorStatus.OFF if value == "0" else SensorStatus.ON

    def toggle_status(self):
        if self.__status == SensorStatus.OFF:
            self.__status = SensorStatus.ON
        else:
            self.__status = SensorStatus.OFF


# Classi create solo per categorizzare i sensori
class Light(Sensor):
    def __init__(self, name, topic_pub, topic_sub) -> None:
        super().__init__(name, topic_pub, topic_sub)
        self.type = "light"


class NGCSensor(Sensor):
    def __init__(self, name, topic_pub, topic_sub) -> None:
        super().__init__(name, topic_pub, topic_sub)
        self.type = "ngc"


class SciSensor(Sensor):
    def __init__(self, name, topic_pub, topic_sub) -> None:
        super().__init__(name, topic_pub, topic_sub)
        self.type = "sci"


class InfoLed(Sensor):
    def __init__(self, name, topic_sub) -> None:
        super().__init__(name, topic_pub=None, topic_sub=topic_sub)
        self.type = "infoled"
