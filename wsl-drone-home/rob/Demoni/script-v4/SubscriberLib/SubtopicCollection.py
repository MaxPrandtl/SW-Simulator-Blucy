from SubscriberLib.Exceptions import SubtopicNotFoundException
from SubscriberLib.SubtopicValue import SubtopicValue
from abc import ABC, abstractmethod

"""
  Classe (astratta) per memorizzare tutti i valori dei subtopic
  Contiene un dizionario come store interno e una serie di metodi per controlli,
  letture ed inserimenti di valori
"""

class SubtopicCollection(ABC):
  def __init__(self, subtopics):
    self.__store = dict()
    
    # Popolo il dizionario con tutti SubtopicValue nulli
    for subtopic in subtopics:
      self.__store[subtopic] = SubtopicValue()

  # Ritorna il valore tramite nome
  def __getitem__(self, key: str) -> SubtopicValue:
    # Controllo che il subtopic esista    
    if not self.__has_subtopic(key):
      raise SubtopicNotFoundException(f'"{key}" is not a valid subtopic')

    return self.__store.get(key)
  
  # Ritorna una lista di tutti i subtopic
  def get_all_subtopics(self):
    return list(self.__store.keys())

  # Ritorna True se tutti gli elementi sono diversi da None
  def is_full(self) -> bool:
    # for _, value in self.__store.items():
    #     if value.is_None:
    #       return False
    # return True
    return all(subtopic_value.value for subtopic_value in self.__store.values())


  # Setta tutti i valore a None
  def reset_all(self) -> None:
    for key, _ in self.__store.items():
      self.__reset_item(key)

  # Controlla che tutti i timestamp siano uguali
  def same_timestamp(self) -> bool:
    all_ts = [subtopic.timestamp for subtopic in self.__store.values()]
    return all(ts == all_ts[0] for ts in all_ts)

  # Verifica se tutti i valori memorizzati sono validi
  def all_valid(self) -> bool:
    # for subtopic_value in self.__store.values():
    #   if not subtopic_value.is_valid:
    #     return False
    # return True
    return all(subtopic_value.is_valid for subtopic_value in self.__store.values())
  
  def set_item(self, subtopic: str, value: SubtopicValue):
    # Controllo che il nome esista
    if not self.__has_subtopic(subtopic):
      raise SubtopicNotFoundException(f'"{subtopic}" is not a valid subtopic')

    # Se la key esiste aggiungo allo store interno
    self.__store[subtopic] = value


  # ---------- Metodi astratti che devono essere implementati da Topic ----------

  @abstractmethod
  def on_message(self, message: str):
    pass

  @abstractmethod
  def on_all_received(self):
    pass

  @abstractmethod
  def parse_data(self, message: str) -> SubtopicValue:
    pass

  # ---------- Metodi privati utility ---------- 

  # Setta il valore di un campo a None
  def __reset_item(self, key: str) -> None:
    self.__store[key].reset()

  # Controlla che il subtopic esista
  def __has_subtopic(self, key: str) -> bool:
    if not key in self.__store.keys():
      return False
    return True


  # ----------  ToStirngs ---------- 

  # Ritorna una stringa completa della collezione
  def complete_log(self) -> str:
    s = ''
    for key, value in self.__store.items():
      s += f'{key}: {value.complete_log()}'
    return s

  # Override __str__
  def __str__(self) -> str:
    s = ''
    for key, value in self.__store.items():
      s += f'{key}: {value}\n'
    return s
