
import asyncio
import os
import signal
import time

from transformations import convert, byte_convert, script_convert
from operator import itemgetter

import json
import socket
from gmqtt import Client as MQTTClient

config = {}

inc = {}
outs = {}
trans = {}
listeners = {}
topics = {}
topic_qos = {}

last_data = {}

def start_conn(conn):
    if conn.get('enabled', True):
        inbound = conn.get('incoming')
        for incoming in conn.get('topics'):
            intopic = ""
            if type(incoming) == str:
                intopic = incoming
            else:
                intopic = incoming.get('topic')
            if intopic not in listeners:
                listeners[intopic] = []
            listeners[intopic].append(conn.get('name'))

        topics[conn.get('name')] = conn.get('incoming')
        inc[conn.get('name')] = {
            "original": conn,
            "trans": conn.get('transformations', []),
            "out": conn.get('outgoing')
        }




#import uvloop
#asyncio.set_event_loop_policy(uvloop.EventLoopPolicy())


STOP = asyncio.Event()



def on_connect(client, flags, rc, properties):
    print('Connected')
    for conn in config.get('connections', []):
        if conn.get('enabled', True):
            for incoming in conn.get('incoming'):
                for out in conn.get('outgoing', []):
                    if out.get('protocol') == "tcp":
                        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                        s.connect((out.get('ip'), int(out.get('port'))))
                        outs[conn.get('name')] = s
                if(type(incoming) == str):
                    print('subscribing', incoming)
                    client.subscribe(incoming)
                else:
                    print('subscribing', incoming)
                    client.subscribe(incoming['topic'])
                    topic_qos[incoming['topic']] = incoming['qos']

            
def on_message(client, topic, payload, qos, properties):
    print('message', topic, payload, qos, properties)
    old_payload = payload
    parts = payload.split(b' ')
    if len(parts) == 3:
        payload = parts[0]
        qos = parts[2]
    elif len(parts) == 1:
        payload = parts[0]
    try:
        qos = int(qos)
    except:
        qos = 0
        payload = old_payload
    print('message', topic, payload, qos, properties)
    if qos >= topic_qos.get(topic, 0):
        last_data[topic] = payload
        conn_names = listeners.get(topic, [])
        print('message to', conn_names)
        for conn_name in conn_names:
            topic_meta = inc.get(conn_name)
            needs = topics.get(conn_name)

            cdata = []
            for var in needs:
                if type(var) == str:
                    cdata.append(last_data.get(var))
                else:
                    cdata.append(last_data.get(var.get('topic')))
            print('cdata', cdata)
            if all(cdata):

                print(topic_meta, cdata)
                data_out = None
                if topic_meta.get('trans'):
                    tconf = topic_meta.get('trans')
                    if tconf.get('mode', 'noop') == "script":
                        params = [last_data.get(var) for var in tconf.get('params')]
                        data_out = convert_script(params, tconf.get('script'))
                    #elif tconf.get('mode', 'noop') == "skip":
                    #    data_out = cdata[10:]
                    elif tconf.get('mode', 'noop') == 'noop':
                        data_out = cdata[0]
                else:
                    data_out = cdata[0]

                print('data_out', data_out)
                for cout in topic_meta.get('out'): #others
                    mode = cout.get('protocol','tcp')
                    if mode == "tcp":
                        outs[conn_name].send(data_out)
                    elif mode == "udp":
                        print(cout)
                        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                        if cout.get('ip') == "255.255.255.255": #broadcast:
                            s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                        s.setblocking(0)
                        s.sendto(data_out, (cout.get('ip'), int(cout.get('port'))))
                    elif mode == "socket":
                        pass




def on_disconnect(client, packet, exc=None):
    print(packet, exc)
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

    for conn in config.get('connections', []):
        start_conn(conn)

if __name__ == '__main__':
    startup(json.load(open('config.json')))