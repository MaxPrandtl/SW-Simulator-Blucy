import threading
import uuid
import multiprocessing

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
        print(message, qos)

    def setup_outgoing(self):
        import outbound
        for out_conf in  self.conf.get('outgoing'):                
            print(out_conf)
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
            fmt = self.trans_conf.get('format')
            fmt = json.load(open(f'formats/{fmt}.json'))

        elif self.trans_conf.get('mode', 'noop') == "script":
            params = [last_data.get(var) for var in tconf.get('params')]
            data_out = convert_script(params, tconf.get('script'))




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

