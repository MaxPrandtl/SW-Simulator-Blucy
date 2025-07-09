from SubscriberLib.Exceptions import RejectedValueException
from SubscriberLib.TopicCollection import TopicCollection
from SubscriberLib.SubtopicValue import SubtopicValue
from SubscriberLib.Topic import Topic
from subprocess import PIPE, Popen

class Subscriber:
  def __init__(self, topics, on_all_data_available=None) -> None:
    self.__topic_collection = TopicCollection(topics)
    self.__on_all_data_available = on_all_data_available 
    # Creazione del processo mosquitto_sub
    self.__create_process()


  def listen(self):
    print('Listening...')
    while True:
      message: str = self.__read_message()

      splitted_message: list[str] = message.split()
      topic_str: str = splitted_message[0].split('/')[0]
      subtopic_str: str = '/'.join(splitted_message[0].split('/')[1:])
      payload: str = ' '.join(splitted_message[1:])

      topic: Topic = self.__topic_collection[topic_str]

      try:
        parsed_message: SubtopicValue = topic.parse_data(payload)
      except RejectedValueException as e:
        parsed_message: SubtopicValue = SubtopicValue(value=e.args[0], valid=False)

      topic.set_item(subtopic_str, parsed_message)
      
      topic.on_message(message)

      topic.on_all_received()
      
      if self.__on_all_data_available != None and self.__topic_collection.all_data_available():
        self.__on_all_data_available(self.__topic_collection)
        self.__topic_collection.reset_all()
        

  # Metodi privati

  def __read_message(self) -> str:
    return self.__process.stdout.readline().strip().decode('utf-8')

  # Crea il processo mosquitto_sub e si iscrive a tutti i topic e subtopic
  def __create_process(self):
    proc_arr = ['mosquitto_sub', '-v', '-p', '1883']
    for topic in self.__topic_collection.get_values():
      for subtopic in topic.get_option_subtopic_list():
        proc_arr.append('-t')
        proc_arr.append(subtopic)

    self.__process = Popen(proc_arr, stdout=PIPE, stderr=PIPE)
    
