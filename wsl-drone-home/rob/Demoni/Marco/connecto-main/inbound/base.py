import threading
import multiprocessing

class BaseInbound():
    def __init__(self, conf, name, logging="print"):
        #super().__init__(None)
        self.conf = conf
        self.name = name
        self.logging = logging

    def run(self):
        self.connect()

    def connect(self):
        pass
    
    def data_received(self, data, qos=0):
        pass
