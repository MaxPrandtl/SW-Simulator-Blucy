from Enum.CommandStatus import BottomCamStatus


class BottomCam:
    def __init__(self) -> None:
        self.net_topic = "ana_dig_io/DO30"
        self.cam_topic1 = "ana_dig_io/DO24"
        self.cam_topic2 = "ana_dig_io/DO26"

        self.__status: BottomCamStatus = BottomCamStatus.OFF

    @property
    def status(self) -> BottomCamStatus:
        return self.__status

    @status.setter
    def status(self, value: BottomCamStatus):
        self.__status = value

    def turn_on(self):
        self.__status = BottomCamStatus.ON
