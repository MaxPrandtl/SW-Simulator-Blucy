from .base import BaseInbound

import serial
import serial.threaded

class Connector(BaseInbound):
    
    
    def connect(self):
        print('connecting')
        self.ser = serial.Serial()
        self.ser.baudrate = self.conf.get('rate')
        self.ser.port = self.conf.get('port')
        self.ser.open()


        ser_to_net = self.SerialToMqtt(client, outgoing, transformations)
        serial_worker = serial.threaded.ReaderThread(ser, ser_to_net)
        serial_worker.start()

    class SerialToMqtt(serial.threaded.Protocol):
        """serial->mqtt"""

        def __init__(self, out_message, outgoing, transformations):
            self.out_message = out_message
            self.out = outgoing
            self.basetopic, self.trans = get_config(transformations)
            self.buffersize = sum([x['bytes'] for x in self.trans['parts']])-7
            print(self.buffersize)
            self.buffer = b''

        def __call__(self):
            return self

        def data_received(self, data):
            print('received', data)
            self.buffer += data

            if len(self.buffer) >= self.buffersize:
                self.send_object(self.buffer[self.buffer.index(bytes([165])):self.buffer.index(bytes([165]))+self.buffersize])
                self.buffer = self.buffer[self.buffersize:]
                print('already in buffer:', len(self.buffer), self.buffer)
                if len(self.buffer) > 0:
                    try:
                        bgn = self.buffer.index(bytes([165]))
                    except:
                        bgn = -1
                    self.buffer = self.buffer[bgn:]

        def send_object(self, data):
            print('transforming', data)
            irs = byte_convert(data, self.trans)
            for topic in self.out:
                self.out_message(topic, data, 0)
            #for k in irs:
            #    tpc = self.basetopic + "/"+k.lower().replace(' ', '_')
            #    print(tpc, irs[k])
            #    self.mqtt.publish(tpc, str(irs[k]))
                