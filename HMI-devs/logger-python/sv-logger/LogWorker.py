from queue import Queue
from threading import Thread
from os import path

class LogWorker(Thread):
  def __init__(self, q, max_size=1000, parent_folder=None) -> None:
    super(LogWorker, self).__init__()
    self.daemon = True
    self.queue: Queue = q
    
    if parent_folder == None:
      raise ValueError('Specificare il nome della cartella in cui salvare i logs')
    elif not path.isdir(parent_folder):
      raise ValueError('Il percorso della cartella non è valido')

    self.parent_folder = parent_folder

    self.current_file = '%s/sv_log.log' % self.parent_folder

  def __log_to_file(self):
    with open(self.current_file, 'a') as f:
      f.write(self.__parse_message(self.queue.get()))

  # TODO: convertire pressure in depth
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
        raise Exception('Errore durante la scrittura del log', e)