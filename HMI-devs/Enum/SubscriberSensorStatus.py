from enum import Enum


class SubscriberSensorStatus(Enum):
    OK = 0
    TIMESTAMP_ERROR = 1
    NOT_VALID = 2
    OFF = 3
    UPD_DISCONNECTED = 4
