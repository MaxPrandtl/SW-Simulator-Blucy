from model.MessageModel import MessageModel


class TelemetryData:
    def __init__(self, cb) -> None:
        self.__data = {}
        self.__cb = cb

    def add_data(self, key, number_of_points=100):
        self.__data[key] = {"x": [], "y": [], "len": number_of_points}

    def get_all_data(self):
        return self.__data

    def update_data(self, key, data: MessageModel):
        x = data.timestamp
        y = data.value

        data_len = len(self.__data[key]["x"])
        if data_len >= self.__data[key]["len"]:
            self.__data[key]["x"] = self.__data[key]["x"][1:]
            self.__data[key]["y"] = self.__data[key]["y"][1:]

        self.__data[key]["x"].append(x)
        self.__data[key]["y"].append(y)

        self.__cb(key, self.__data[key])
