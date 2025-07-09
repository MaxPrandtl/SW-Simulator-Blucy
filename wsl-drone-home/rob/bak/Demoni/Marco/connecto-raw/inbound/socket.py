from .base import BaseInbound

import socket

class Connector(BaseInbound):
    def connect(self):
        if self.conf.get('protocol') == "tcp":
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((out.get('host', '127.0.0.1'), int(out.get('port'))))
            self.connection = s
        if self.conf.get('protocol') == "udp":
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            #s.connect((out.get('ip'), int(out.get('port'))))
            self.connection = s