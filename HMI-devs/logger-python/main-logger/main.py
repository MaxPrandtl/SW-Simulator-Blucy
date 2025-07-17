from pathlib import Path
import sys
import paho.mqtt.client as mqtt
from queue import Queue
from sys import argv
from LogWorker import LogWorker
from FogListenerWorker import FogListenerWorker
from json import load

PORT = 1883

if len(argv) != 4:
  print('Numero di argomenti non corretto')
  exit(2)

config_dir = Path.home().joinpath('.blucy-logger')
config_file_path = config_dir.joinpath('config.json')

with open(config_file_path, 'r') as f:
  data = load(f)
  topics = data.get('topics')
  ip_broker = data.get('broker-ip')
  ground_ip = data.get('ground-machine-ip')


messages_queue = Queue(maxsize=-1)

def get_selected_topics():
 
  topics_list = []

  for topic in topics:
    topics_list.append((topic,0))

  return topics_list
  
def on_connect(client, userdata, flags, rc):
  print('Connected')
  topics = get_selected_topics()
  print(topics)
  client.subscribe(topics)

def on_message(client, userdata, msg):
  messages_queue.put([msg.topic, msg.payload])

try:
  if argv[3] == '1':
    fog_listener = FogListenerWorker(q=messages_queue, ground_ip=ground_ip)
    fog_listener.start()
  logger = LogWorker(messages_queue, parent_folder=argv[1], max_size=int(argv[2]))
  logger.start()
  print("running log")

  print("running mqtt client")
  client = mqtt.Client()
  print("running mqtt client")
  client.on_connect = on_connect
  print("running mqtt client")
  client.on_message = on_message
  print("running mqtt client")

  # Ip drone
  client.connect(ip_broker, PORT, 60)

  client.loop_forever()
except KeyboardInterrupt as e:
  print('\nExit...')
  exit(0)
except ValueError as e:
  print(e)
  exit(1)
except Exception as e:
  sys.stderr.write(f'Errore di connessione al broker.\nhost: {ip_broker}\nport: {PORT}\n{e}')
print('---')