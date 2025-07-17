from model.MessageModel import MessageModel


class MotorStatusData:
    def __init__(self, cb) -> None:
        self.__cb = cb
        self.__data = {
            "surge": 0.0,
            "sway": 0.0,
            "heave": 0.0,
            "yaw": 0.0,
        }

    def get_data(self, key):
        return self.__data[key]

    def update_data(self, key, data: MessageModel):
        value = data.value
        self.__data[key] = value

        self.__cb(key)
