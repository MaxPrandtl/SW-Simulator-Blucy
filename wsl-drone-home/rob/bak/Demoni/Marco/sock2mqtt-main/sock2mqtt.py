import asyncio
import os
import signal
import time

from transform import *

import json
import socket
from gmqtt import Client as MQTTClient

import threading

config = json.load(open('config.json'))

inc = {}
out = {}
direct = {}
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
                t = threading.Thread(target=do_server_serial, args=(client, incoming, conn.get('outgoing'), conn.get('direct'), conn.get('transformations')))
                t.start()

def get_config(conf):
    return conf['baseTopic'], json.load(open(conf['external']))


def do_server_serial(client, incoming, outgoing, direct, transformations):
    ser = serial.Serial()
    ser.baudrate = incoming.get('rate')
    ser.port = incoming.get('port')
    ser.open()

    ser_to_net = SerialToMqtt(client, outgoing, direct,  transformations)
    serial_worker = serial.threaded.ReaderThread(ser, ser_to_net)
    serial_worker.start()


class SerialToMqtt(serial.threaded.Protocol):
    """serial->mqtt"""

    def __init__(self, client, outgoing, direct, transformations):
        self.mqtt = client
        self.out = outgoing
        self.direct = direct
        if self.direct.get('protocol', 'tcp') == 'udp':
            self.dsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            if self.direct.get('host', '127.0.0.1') == "255.255.255.255": #broadcast:
                self.dsocket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            self.dsocket.setblocking(0)    
        elif self.direct.get('protocol', 'tcp') == 'tcp':
            self.dsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.dsocket.connect((self.direct.get('host'), int(self.direct.get('port'))))
        self.basetopic, self.trans = get_config(transformations)
        self.buffersize = sum([x['bytes'] for x in self.trans['parts']])-4
        print(self.buffersize)
        self.buffer = b''

    def __call__(self):
        return self

    def data_received(self, data):
        print('received', data)
        print(self.buffersize)
        self.buffer += data

        if len(self.buffer) >= self.buffersize:
            data = self.buffer[self.buffer.index(b'\xa5'):self.buffer.index(b'\xa5')+self.buffersize]
            cted = convert(data, self.trans)
            self.send_object(data)
            self.buffer = self.buffer[self.buffersize:]
            print('already in buffer:', len(self.buffer), self.buffer)
            if len(self.buffer) > 0:
                try:
                    bgn = self.buffer.index(b'\xa5')
                except:
                    bgn = 0
                self.buffer = self.buffer[bgn:]

    def send_object(self, data, mode=27):
        print('transforming', data)
        try:
            d = threading.Thread(target=self.send_direct, args=(data, ))
            d.start()
        except:
            print('error in udp')

        try:
            m = threading.Thread(target=self.send_mqtt, args=(data,))
            m.start()
        except:
            print('error in mqtt')

    def send_mqtt(self, data):
        irs = convert(data, self.trans)
        for topic in self.out:
            self.mqtt.publish(topic, data)
        #for k in irs:
        #    tpc = self.basetopic + "/"+k.lower().replace(' ', '_')
        #    print(tpc, irs[k])
        #    try:
        #        self.mqtt.publish(tpc, str(irs[k]), 0)
        #    except:
        #        pass

    def send_direct_50006(self, data):
        self.dsocket.sendto(data, (self.direct.get('host'), 50006))

    def send_direct(self, data):
        if self.direct.get('protocol', 'tcp') == 'udp':
            self.dsocket.sendto(data, (self.direct.get('host'), int(self.direct.get('port'))))
        else:
            self.dsocket.sendall(data)

            
def on_message(client, topic, payload, qos, properties):
    topic_meta = inc.get(topic)
    trans = convert(payload, get_config())
    print(trans)
    if topic_meta.get('trans'):
        print(topic_meta.get('trans'))
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
        elif mdoe == "socket":
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


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    host = config.get('host')
    loop.add_signal_handler(signal.SIGINT, ask_exit)
    loop.add_signal_handler(signal.SIGTERM, ask_exit)
    loop.run_until_complete(main(host))
