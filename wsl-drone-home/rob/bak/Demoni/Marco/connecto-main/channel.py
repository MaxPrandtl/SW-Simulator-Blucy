import threading
import uuid
import multiprocessing
from transformations import byte_convert, script_convert

class Channel(multiprocessing.Process):
    def __init__(self, conf):
        super().__init__()
        self.gconf = conf
        self.conf = conf.get('connections')[0]
        self.outgoing = {}

    def setup_incoming(self):
        import inbound
        in_conf = self.conf.get('incoming')
        print(in_conf)
        if in_conf.get('protocol') == "serial":
            self.incoming = inbound.serial.Connector(in_conf, self.conf.get('name'))
        elif in_conf.get('protocol') in ["tcp", "udp"]:
            self.incoming = inbound.socket.Connector(in_conf, self.conf.get('name'))
        elif in_conf.get('protocol') == "mqtt":
            self.incoming = inbound.mqtt.Connector(in_conf, self.conf.get('name'))
        self.incoming.data_received = self.on_message
        self.incoming.run()

    def on_message(self, message, qos):
        print('on_message', message, qos)
        out = self.do_transform(message)
        print('out', out) 
        self.emit(out)

    def emit(self, message):
        print(self.outgoing)
        for out in self.outgoing.keys():
            print('out:', out)
            self.outgoing[out].send_msg(message)

    def setup_outgoing(self):
        import outbound
        for out_conf in  self.conf.get('outgoing'):                
            print(out_conf)
            if out_conf.get('enabled', True):
                if out_conf.get('protocol') == "serial":
                    out = outbound.serial.Connector(out_conf, self.conf.get('name'))
                    out.run()
                    self.outgoing[out_conf.get('name', str(uuid.uuid4()))] = out
                elif out_conf.get('protocol') in ["tcp", "udp"]:
                    out = outbound.socket.Connector(out_conf, self.conf.get('name'))
                    out.run()
                    self.outgoing[out_conf.get('name', str(uuid.uuid4()))] = out
                elif out_conf.get('protocol') == "mqtt":
                    out = outbound.mqtt.Connector(out_conf, self.conf.get('name'))
                    out.run()
                    self.outgoing[out_conf.get('name', str(uuid.uuid4()))] = out

    def setup_transformation(self):
        self.trans_conf = self.conf.get('transformation')
        print(self.trans_conf)

        if self.trans_conf.get('mode', 'noop') == "format": 
            self.trans_mode = 'format'
            fmt = self.trans_conf.get('format')
            self.trans_fmt = json.load(open(f'formats/{fmt}.json'))

        elif self.trans_conf.get('mode', 'noop') == "script":
            self.trans_mode = 'script'
            self.trans_script = self.trans_conf.get('script')
            self.trans_params = self.trans_conf.get('params')
        
        elif self.trans_conf.get('mode', 'noop') == "noop":
            self.trans_mode = 'noop'

    def do_transform(self, data):
        if self.trans_mode == "noop":
            return data

        elif self.trans_mode == "format":
            return byte_convert(data, self.trans_fmt)
        
        elif self.trans_mode == "script":
            if type(data) == list:
                params = data
            else: 
                params = [data.get(var) for var in self.trans_params]
            out_data = script_convert(params, self.trans_script)
            print(out_data)
            return out_data



    def startup(self):
        self.setup_outgoing()
        self.setup_transformation()
        self.setup_incoming()

    def run(self):
        self.startup()
        

if __name__ == "__main__":
    import json
    j = json.load(open('config.json'))
    c = Channel(j)
    c.start()

