from SubscriberLib import SubtopicValue
from SubscriberLib import RejectedValueException

def parser(msg: str) -> SubtopicValue:
  data = msg.split()
  
  if len(data) != 3:
    raise RejectedValueException('Format error')

  # Se il bit di controllo è 0 scarto il dato
  if data[2] == '0':
    raise RejectedValueException('Control bit is 0')

  return SubtopicValue(value=data[0], timestamp=data[1])
