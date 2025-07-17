from enum import Enum


class SensorStatus(Enum):
    OFF = 0
    ON = 1


class InfoLedStatus(Enum):
    OFF = 0
    ON = 1
    WARN = 2
    NO_COLOR = 3


class BottomCamStatus(Enum):
    OFF = 0
    ON = 1
    SHUTTING_DOWN = 2


class AltitudeStatus(Enum):
    SAFE = 0
    WARNING = 1
    DANGER = 2
