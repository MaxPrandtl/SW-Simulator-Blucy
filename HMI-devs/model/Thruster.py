from Enum.ThrusterSatus import ThrusterStatus


class Thruster:
    def __init__(self, name, di, enable_status) -> None:
        self.__name = name
        self.__di = di
        self.__enable_status = enable_status

        self.first_command = False
        self.second_command = False

    @property
    def enable_status(self):
        return self.__enable_status

    @property
    def name(self):
        return self.__name

    @property
    def di(self):
        return self.__di

    @property
    def status(self):
        if self.first_command and self.second_command:
            return ThrusterStatus.ON
        else:
            return ThrusterStatus.OFF
