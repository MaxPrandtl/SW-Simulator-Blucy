from .base import BaseInbound

import threading

import socket

class Connector(BaseInbound):
    def connect(self):
        if self.conf.get('protocol') == "tcp":
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((self.conf.get('host', '127.0.0.1'), int(self.conf.get('port'))))
            self.connection = s
        if self.conf.get('protocol') == "udp":
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.bind(('', int(self.conf.get('port'))))
            self.connection = s
        self.get_data()


    def get_data(self):
        while True:
            data, _ = self.connection.recvfrom(1024)
            self.data_received(data.decode(), 0)
