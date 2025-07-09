from .base import BaseOutbound

import socket

class Connector(BaseOutbound):
    
    def connect(self):
        if self.conf.get('protocol') == "tcp":
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.conf.get('host', '127.0.0.1'), int(self.conf.get('port'))))
        elif self.conf.get('protocol') == "udp":
            print('creating socket for udp')
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            if self.conf.get('host', '127.0.0.1') == "255.255.255.255": #broadcast:
                self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            self.socket.setblocking(0)
            print('scoket udp rected')

    def send_msg(self, message, *args):
        #print('sending message', message, self.conf)
        if self.conf.get('protocol') == 'udp':
            #print(self.conf.get('host', '127.0.0.1'), int(self.conf.get('port')))
            self.socket.sendto(message.encode('utf-8'), (self.conf.get('host', '127.0.0.1'), int(self.conf.get('port'))))
        if self.conf.get('protocol') == 'tcp':
            self.socket.send(message.encode('utf-8'))
    
    
