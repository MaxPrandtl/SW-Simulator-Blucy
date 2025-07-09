from SubscriberLib.Exceptions import TopicNotFoundException
from SubscriberLib.Topic import Topic


class TopicCollection:
  def __init__(self, topics) -> None:
    self.__store = dict()

    for topic in topics:
      self.__store[topic.main_topic] = topic
  
  def __getitem__(self, key: str) -> Topic:
    try:
      return self.__store[key]
    except:
      raise TopicNotFoundException('Topic "%s" not found' % key)

  def get_values(self):
    return self.__store.values()
  
  def all_data_available(self) -> bool:
    for topic in self.get_values():
      if not topic.is_full():
        return False
    return True

  def reset_all(self):
    for topic in self.get_values():
      topic.reset_all()

  def complete_log(self) -> str:
    s = ''
    for topic in self.get_values():
      s += topic.complete_log()
    return s
    
  def __str__(self) -> str:
    s = ''
    for topic in self.get_values():
      s += str(topic)
    return s
