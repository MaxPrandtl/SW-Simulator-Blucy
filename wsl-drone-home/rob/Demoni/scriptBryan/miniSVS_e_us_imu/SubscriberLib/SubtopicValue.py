"""
  Classe che modella un Subtopic e contiene il valore e il timestamp
"""


class SubtopicValue:
  def __init__(self, timestamp: str = None, value: str = None, valid=True) -> None:
    self.__value = value
    self.__timestamp = timestamp
    self.__valid = valid
  
  @property
  def value(self):
    return self.__value

  @value.setter
  def value(self, val: str):
    self.__value = str(val)

  @property
  def timestamp(self):
    return self.__timestamp
  
  @timestamp.setter
  def timestamp(self, val: str):
    self.__timestamp = str(val)
  
  @property
  def is_valid(self) -> bool:
    return self.__valid
  
  @is_valid.setter
  def is_valid(self, val: bool):
    self.__valid = val if type(val) == bool else False

  @property
  def is_None(self):
    return self.__value == None
  

  def reset(self):
    self.__valid = True
    self.__value = None
    self.__timestamp = None
  

  def complete_log(self):
    return f'value: {self.__value}\ntimestamp: {self.__timestamp}\n'

  def __str__(self) -> str:
    return self.value if self.value != None else 'None'