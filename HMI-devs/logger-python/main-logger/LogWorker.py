from genericpath import isfile
from queue import Queue
from threading import Thread
from os import path
from pathlib import Path

class LogWorker(Thread):
  def __init__(self, q, max_size=1000, parent_folder=None) -> None:
    super(LogWorker, self).__init__()
    self.daemon = True
    self.data_counter = 0
    self.file_counter = 0
    self.queue: Queue = q
    self.max_size = max_size
    
    if parent_folder == None:
      raise ValueError('Specificare il nome della cartella in cui salvare i logs')
    elif not path.isdir(parent_folder):
      raise ValueError('Il percorso della cartella non è valido')

    self.parent_folder = parent_folder

    self.__update_current_file()


  def __update_current_file(self):
    self.current_file = Path(self.parent_folder, 'log%0*d.log' % (3, self.file_counter))
    #self.current_file = '%slog%0*d.log' % (self.parent_folder, 3, self.file_counter)
    #print(self.current_file)


  def __log_to_file(self):
    if self.data_counter > self.max_size - 1:
      self.file_counter += 1
      self.data_counter = 0
      self.__update_current_file()
      

    with open(self.current_file, 'a') as f:
      f.write(self.__parse_message(self.queue.get()))
      self.data_counter += 1


  def __parse_message(self, message):
    topic = message[0]
    payload = message[1] if isinstance(message[1], str) else message[1].decode('utf-8')
    return '%s %s\n' % (topic, payload)


  def run(self) -> None:
    while True:
      try:
        if not self.queue.empty():
          self.__log_to_file()
      except Exception as e:
        if not path.isfile(self.current_file):
          raise Exception(f'{self.current_file} is not a file\ndata_counter:{self.data_counter}')
        continue
        raise Exception(f'Errore durante la scrittura del log\n{e}\n{self.current_file}\n{self.data_counter}')