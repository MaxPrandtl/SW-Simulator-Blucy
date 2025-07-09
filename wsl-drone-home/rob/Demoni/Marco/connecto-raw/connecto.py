import json

import threading
import multiprocessing

from sock2mqtt import startup as sock2mqtt
from mqtt2sock import startup as mqtt2sock

from channel import Channel

class Connecto():
    def __init__(self, conf):
        self.conf = json.load(open(conf))
        self.channels = {}

    def start(self):
        print('starting connections for', self.conf.get('id'))

        for conn in self.conf.get('connections'):
            if conn.get('enabled', True):
                try:
                    print('starting connection', conn.get('name'))

                    conn_conf = {
                        "id": self.conf.get('id')+"__"+conn.get('name').replace(' ', '_'),
                        "connections": [
                            conn
                        ]
                    }
                    print('creating channel for', conn.get('name'))
                    self.channels[conn.get('name')] = Channel(conn_conf)
                    print('starting channel for', conn.get('name'))
                    self.channels[conn.get('name')].start()
                    print('channel started:', conn.get('name'))
                        
                except Exception as ex: 
                    print(ex)
        
        for conn in self.conf.get('connections'):
            print('waiting for:', conn.get('name'))
            self.channels[conn.get('name')].join()

if __name__ == "__main__":
    conn = Connecto('config.json')
    conn.start()