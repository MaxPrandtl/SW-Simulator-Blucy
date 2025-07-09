import threading

class BaseInbound(threading.Thread):
    def __init__(self, conf, name):
        super().__init__(None)
        self.conf = conf
        self.name = name

    def run(self):
        self.connect()

    def connect(self):
        pass
    
    def data_received(self, data, qos):
        pass