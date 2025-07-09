from .base import BaseInbound

import serial
import serial.threaded

import threading
import json

class Connector(BaseInbound):
    
    def out_message(data, qos):
        print(data, qos)
        self.data_received(data, qos)

    def connect(self):
        print('connecting')
        print('config:', self.conf)
        self.ser = serial.Serial()
        self.ser.baudrate = self.conf.get('rate')
        self.ser.port = self.conf.get('port')
        self.ser.open()

        fmt = self.conf.get('format')
        fmt = json.load(open(f'formats/{fmt}.json'))    

        self.ser_to_net = SerialToMqtt(self.out_message, fmt)
        self.serial_worker = serial.threaded.ReaderThread(self.ser, self.ser_to_net)
        self.serial_worker.daemon=False
        self.serial_worker.start()
        print('connected')

class SerialToMqtt(serial.threaded.Protocol):
    """serial->mqtt"""

    def __init__(self, out_message, fmt):
        print('connector up')
        print(out_message, fmt)
        self.out_message = out_message
        self.trans = fmt
        #self.basetopic, self.trans = get_config(transformations)
        self.buffersize = sum([x['bytes'] for x in self.trans['parts']])-7
        print(self.buffersize)
        self.buffer = b''

    def __call__(self):
        print('call')
        return self

    def connection_made(self, transport):
        print('connection_made')
        print(transport)

    def connection_lost(self, exc):
        print(exc)

    def data_received(self, data):
        #print('received', data)
        self.buffer += data
        if len(self.buffer) >= self.buffersize:
            self.send_object(self.buffer[self.buffer.index(bytes([165])):self.buffer.index(bytes([165]))+self.buffersize])
            self.buffer = self.buffer[self.buffersize:]
            #print('already in buffer:', len(self.buffer), self.buffer)
            if len(self.buffer) > 0:
                try:
                    bgn = self.buffer.index(bytes([165]))
                except:
                    bgn = -1
                self.buffer = self.buffer[bgn:]

    def send_object(self, data):
            #print('sending object', data)
            irs = byte_convert(data, self.trans)
            self.out_message(data, 0)
            #for k in irs:
            #    tpc = self.basetopic + "/"+k.lower().replace(' ', '_')
            #    print(tpc, irs[k])
            #    self.mqtt.publish(tpc, str(irs[k]))
                
