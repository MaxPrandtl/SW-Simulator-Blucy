from model.MessageModel import MessageModel

""" Contenitore di MessgeModel relativi ai dati di miniSVS e miniCT"""


class CTDDataModel:
    def __init__(self) -> None:
        self.pressure = MessageModel()
        self.soundSpeed = MessageModel()
        self.temperature = MessageModel()
        self.conductivity = MessageModel()
