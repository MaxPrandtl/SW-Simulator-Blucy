from queue import Queue
from threading import Thread
from decimal import Decimal
import socket
import sys

class FogListenerWorker(Thread):
  def __init__(self, q, ground_ip) -> None:
    super(FogListenerWorker, self).__init__()
    self.daemon = True
    self.queue: Queue = q
    self.port = 2222
    self.host = ground_ip
    self.running = False

    try:
      self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      self.sock.bind((self.host, self.port))
      sys.stdout.write('[MAIN LOGGER]: fog listener socket creata')
    except:
      raise Exception('[MAIN LOGGER]: socket non connessa al fog. X e Y non disponibili.')
      

  def run(self) -> None:
    print('[MAIN LOGGER]: fog listener worker avviato')
    self.running = True
    while self.running:
      try:
        data = self.sock.recv(2048).decode('utf-8')
        data = data.split('&')

        self.queue.put(('fog/y', f'{data[1]}'))
        self.queue.put(('fog/x', f'{data[0]}'))

      except Exception as e:
        print('[MAIN LOGGER]: errore udp fog listener', e)
        continue

    print('[MAIN LOGGER]: fog listener worker terminato')

  