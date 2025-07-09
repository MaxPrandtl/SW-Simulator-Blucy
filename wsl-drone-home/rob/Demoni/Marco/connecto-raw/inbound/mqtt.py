from .base import BaseInbound
import asyncio
import os
import signal

from gmqtt import Client as MQTTClient

class Connector(BaseInbound):
    STOP = asyncio.Event()
    last_data = {}
    
    async def main(self, host):
        print('setting up inbound Mqtt Client')
        print(self.conf)
        self.client = MQTTClient(self.name.replace(' ','_'))

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect
        self.client.on_subscribe = self.on_subscribe
        await self.client.connect(host)
        await self.STOP.wait()
        await self.client.disconnect()

    def connect(self):
        print('setting up inbound mqtt connection')
        host = self.conf.get('host', '127.0.0.1')
        loop = asyncio.get_event_loop()
        loop.run_until_complete(self.main(host))

    def on_connect(self, client, flags, rc, properties):
        print('Connected')
        for incoming in self.conf.get('topics'):
            if(type(incoming) == str):
                print('subscribing', incoming)
                client.subscribe(incoming)
            else:
                print('subscribing', incoming)
                client.subscribe(incoming['topic'])
                topic_qos[incoming['topic']] = incoming['qos']

                
    def on_message(self, client, topic, payload, qos, properties):
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
            self.last_data[topic] = payload
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
                    self.data_received(cdata)


    def on_disconnect(self, client, packet, exc=None):
        print('Disconnected')

    def on_subscribe(self, client, mid, qos, properties):
        print('SUBSCRIBED')

    def ask_exit(self, *args):
        self.STOP.set()