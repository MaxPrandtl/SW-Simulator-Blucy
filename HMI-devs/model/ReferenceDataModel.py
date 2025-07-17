from model.MessageModel import MessageModel


class ReferenceDataModel:
    def __init__(self) -> None:
        self.z = MessageModel()
        self.alt = MessageModel()
        self.psi = MessageModel()
