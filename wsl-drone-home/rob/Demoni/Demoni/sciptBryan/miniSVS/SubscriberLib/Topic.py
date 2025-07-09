from SubscriberLib.SubtopicCollection import SubtopicCollection
from SubscriberLib.SubtopicValue import SubtopicValue


class Topic(SubtopicCollection):
  def __init__(self, main_topic: str, subtopics: list[str], on_message=None, on_all_received=None, data_parser=None) -> None:
    super().__init__(subtopics)
    self.__main_topic = main_topic
    self.__subtopics = subtopics
    self.__on_message = on_message
    self.__on_all_received = on_all_received  
    self.__data_parser = data_parser

  @property
  def main_topic(self):
    return self.__main_topic

  def __getitem__(self, val: str) -> SubtopicValue:
    # return self.__subtopic_collection[val]
    return super().__getitem__(val)

  def parse_data(self, msg: str) -> SubtopicValue:
    if self.__data_parser == None:
      return SubtopicValue(value=msg)
    
    parsed_value = self.__data_parser(msg)

    if isinstance(parsed_value, SubtopicValue):
      return parsed_value
    else:
      raise ValueError('Data parser function must return a SubtopicValue instance')

  def on_message(self, message: str):
    if self.__on_message != None:
      self.__on_message(self, message)
  
  def on_all_received(self):
    if self.__on_all_received != None and self.is_full():
      self.__on_all_received(self)


  # Ritorna un array contenente stringhe nella forma mainTopic/subtopic utile
  # nella creazione del processo mosquitto sub
  def get_option_subtopic_list(self) -> list[str]:
    return [f'{self.main_topic}/{subtopic}' for subtopic in self.__subtopics]

  def __str__(self) -> str:
      return f'\n-------------------------\n{self.main_topic}:\n{super().__str__()}-------------------------'