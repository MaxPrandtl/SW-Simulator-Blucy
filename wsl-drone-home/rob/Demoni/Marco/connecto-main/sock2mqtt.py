import asyncio
import os
import signal
import time

from transformations import convert, byte_convert, script_convert

import json
import socket
from gmqtt import Client as MQTTClient

import threading

config = {}

inc = {}
out = {}
trans = {}

import serial
import serial.threaded

#import uvloop
#asyncio.set_event_loop_policy(uvloop.EventLoopPolicy())


STOP = asyncio.Event()


def on_connect(client, flags, rc, properties):
    print('Connected')
    setup_listeners(client)

def setup_listeners(client):
    for conn in config.get('connections', []):
        for incoming in conn.get('incoming'):
            if incoming.get('mode', 'serial') == "serial":
                t = threading.Thread(target=do_server_serial, args=(client, incoming, conn.get('outgoing'), conn.get('transformations')))
                t.start()

def get_config(conf):
    return conf['baseTopic'], json.load(open("transformations/"+conf['external']))


def do_server_serial(client, incoming, outgoing, transformations):
    ser = serial.Serial()
    ser.baudrate = incoming.get('rate')
    ser.port = incoming.get('port')
    ser.open()

    ser_to_net = SerialToMqtt(client, outgoing, transformations)
    serial_worker = serial.threaded.ReaderThread(ser, ser_to_net)
    serial_worker.start()

class SerialToMqtt(serial.threaded.Protocol):
    """serial->mqtt"""

    def __init__(self, client, outgoing, transformations):
        self.mqtt = client
        self.out = outgoing
        self.basetopic, self.trans = get_config(transformations)
        self.buffersize = sum([x['bytes'] for x in self.trans['parts']])-7
        print(self.buffersize)
        self.buffer = b''

    def __call__(self):
        return self

    def data_received(self, data):
        #print('received', data)
        self.buffer += data

        if len(self.buffer) >= self.buffersize:
            self.send_object(self.buffer[self.buffer.index(b'\xa5'):self.buffer.index(b'\xa5')+self.buffersize])
            self.buffer = self.buffer[self.buffersize:]
            #print('already in buffer:', len(self.buffer), self.buffer)
            if len(self.buffer) > 0:
                try:
                    bgn = self.buffer.index(b'\xa5')
                except:
                    bgn = -1
                self.buffer = self.buffer[bgn:]

    def send_object(self, data):
        #print('transforming', data)
        irs = byte_convert(data, self.trans)
        for topic in self.out:
            self.mqtt.publish(topic, data)
        #for k in irs:
        #    tpc = self.basetopic + "/"+k.lower().replace(' ', '_')
        #    print(tpc, irs[k])
        #    self.mqtt.publish(tpc, str(irs[k]))
            
          
def on_message(client, topic, payload, qos, properties):
    topic_meta = inc.get(topic)
    trans = byte_convert(payload, get_config())
    #print(trans)
    if topic_meta.get('trans'):
        #print(topic_meta.get('trans'))
        pass
    for cout in topic_meta.get('out'):
        mode = cout.get('protocol','tcp')
        if mode == "tcp":
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((TCP_IP, TCP_PORT))
            s.send(payload.decode())
            s.close()
        elif mode == "udp":
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.sendto(payload.decode(), (TCP_IP, TCP_PORT))
        elif mode == "socket":
                pass




def on_disconnect(client, packet, exc=None):
    print('Disconnected')

def on_subscribe(client, mid, qos, properties):
    print('SUBSCRIBED')

def ask_exit(*args):
    STOP.set()


async def main(broker_host):
    client = MQTTClient(config.get('id'))

    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect
    client.on_subscribe = on_subscribe


    await client.connect(broker_host)
    await STOP.wait()
    await client.disconnect()

def startup(conf):
    config = conf
    loop = asyncio.get_event_loop()
    host = config.get('host')
    loop.add_signal_handler(signal.SIGINT, ask_exit)
    loop.add_signal_handler(signal.SIGTERM, ask_exit)
    loop.run_until_complete(main(host))

if __name__ == '__main__':
    startup(json.load(open('config.json')))
    