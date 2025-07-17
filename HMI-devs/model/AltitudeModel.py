from Enum.CommandStatus import AltitudeStatus
from model.MessageModel import MessageModel


class AltitudeModel:
    def __init__(self, low_th, high_th) -> None:
        self.altitude = MessageModel()
        self.__state: AltitudeStatus = None
        self.__thresholds = (float(low_th), float(high_th))

    @property
    def state(self):
        if self.altitude.value <= self.__thresholds[0]:
            self.__state = AltitudeStatus.DANGER
        elif self.__thresholds[0] < self.altitude.value <= self.__thresholds[1]:
            self.__state = AltitudeStatus.WARNING
        else:
            self.__state = AltitudeStatus.SAFE

        return self.__state

    def set_thresholds(self, low_th=None, high_th=None):
        new_low = low_th if low_th is not None else self.__thresholds[0]
        new_high = high_th if high_th is not None else self.__thresholds[1]
        self.__thresholds = (new_low, new_high)
