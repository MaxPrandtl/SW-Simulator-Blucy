from .base import BaseOutbound

class Connector(BaseOutbound):
    
    def __init__(self, conf):
        self.conf = conf

    
    