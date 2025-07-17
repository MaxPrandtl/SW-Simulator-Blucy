from model.MessageModel import MessageModel


class SeaLevelVsDepthData:
    def __init__(self, cb) -> None:
        self.__z = 0.0
        self.__altitude = 0.0
        self.__callback = cb
        self.__z_reference = 0.0

    def __exec_calback(self):
        self.__callback(self.__z, self.sealevel, self.__z_reference)

    @property
    def z_reference(self):
        return self.__z_reference

    @z_reference.setter
    def z_reference(self, z_ref: MessageModel):
        self.__z_reference = z_ref.value
        self.__exec_calback()

    @property
    def z(self):
        return self.__z

    @z.setter
    def z(self, z_: MessageModel):
        self.__z = z_.value
        self.__exec_calback()

    @property
    def altitude(self):
        return self.__altitude

    @altitude.setter
    def altitude(self, altitude_: MessageModel):
        self.__altitude = altitude_.value
        self.__exec_calback()

    @property
    def sealevel(self):
        return self.z + self.altitude
