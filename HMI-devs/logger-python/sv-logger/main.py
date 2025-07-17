from pathlib import Path
import sys
import paho.mqtt.client as mqtt
from queue import Queue
from sys import argv
from LogWorker import LogWorker
from json import load

PORT = 1883

if len(argv) != 2:
    raise Exception("Numero di argomenti non corretto")

config_dir = Path.home().joinpath(".blucy-logger")
config_file_path = config_dir.joinpath("config.json")

with open(config_file_path, "r") as f:
    data = load(f)
    ip_broker = data.get("broker-ip")
    ground_ip = data.get("ground-machine-ip")


messages_queue = Queue(maxsize=-1)


def on_connect(client, userdata, flags, rc):
    print("Connected")
    topics_list = []
    topics_list.append(("miniSVS/pressure", 1))
    topics_list.append(("miniSVS/soundSpeed", 1))
    topics_list.append(("miniCT/temperature", 1))
    topics_list.append(("miniCT/conductivity", 1))

    client.subscribe(topics_list)


def on_message(client, userdata, msg):
    messages_queue.put((msg.topic, msg.payload))


try:
    logger = LogWorker(messages_queue, parent_folder=argv[1])
    logger.start()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    # Ip drone
    client.connect(ip_broker, PORT, 60)

    client.loop_forever()
except KeyboardInterrupt:
    print("\nExit...")
    exit(0)
except ValueError as e:
    print(e)
    exit(1)
except Exception as e:
    sys.stderr.write(f"[SVLOGGER]: Errore di connessione al broker.\nhost: {ip_broker}\nport: {PORT}\n{e}")
print("---")
