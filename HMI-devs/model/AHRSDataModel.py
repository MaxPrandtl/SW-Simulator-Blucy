from model.MessageModel import MessageModel
from utils.DataProcess import Data


class AHRSDataModel:
    def __init__(self) -> None:
        self.heading = MessageModel()
        self.heading.set_processing_function(self.convert_heading_angle)
        self.board_position_angle = 0
        self.roll = MessageModel()
        self.pitch = MessageModel()

    # Converte angolo in range 0-360
    def convert_heading_angle(self, heading: MessageModel):
        return MessageModel(
            Data.convert_angle(heading.value), heading.timestamp, heading.valid
        )
