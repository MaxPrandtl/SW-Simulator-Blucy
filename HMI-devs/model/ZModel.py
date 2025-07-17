from model.MessageModel import MessageModel


class ZModel:
    z_offset = 0

    def __init__(self) -> None:
        self.z = MessageModel()
        self.z.set_processing_function(self.add_offset)

    def reset(self) -> None:
        self.z.value = 0
        self.z.timestamp = 0
        self.z.valid = True

    # Aggiunge un offset alla z
    def add_offset(self, z_: MessageModel):
        return MessageModel(z_.value + ZModel.z_offset, z_.timestamp, z_.valid)

    @staticmethod
    def set_z_offset(value):
        ZModel.z_offset = value
