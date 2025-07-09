from .base import BaseOutbound
import asyncio
import os
import signal

from gmqtt import Client as MQTTClient

class Connector(BaseOutbound):
    STOP = asyncio.Event()
    
    async def main(self, host):
        print('setting up outbound Mqtt Client')
        
        self.client = MQTTClient(self.name.replace(' ','_'))
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        await self.client.connect(host)
        await self.STOP.wait()
        await self.client.disconnect()


    def connect(self):
        print('setting up outbound mqtt connection')
        host = self.conf.get('host', '127.0.0.1')
        loop = asyncio.get_event_loop()
        loop.run_until_complete(self.main(host))

    def send_msg(self, message, *args):
        self.client.publish(topic, data)

    def on_connect(self, client, flags, rc, properties):
        print('Connected')

    def on_disconnect(self,client, packet, exc=None):
        print('Disconnected')

