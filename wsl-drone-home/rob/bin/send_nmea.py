import sys
sys.path.append('/usr/share/python3-mscl/')
from mscl import *
from gmqtt import Client as MqttClient

from decimal import *
from math import floor
from datetime import *
import asyncio
import os
import signal
import time
#TODO: change these constants to match your setup
import socket
STOP = asyncio.Event()
import datetime
import json

import csv

class MSCL2NMEA():
    def __init__(self, mqtt_topic="us_imu"):
        self.mqtt_topic = mqtt_topic
        self.current = {}

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)  # UDP
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    def value(self, topic, payload):
        '''store and overwrite current data with new topic based data'''
        q = payload.decode().split(" ")[-1]
        data = " ".join(payload.decode().split(" ")[0:-2])
        self.current[topic.split("/")[-1]] = {"value": data, "quality": q}

    async def start(self, mqtt, millis):
        self.mqtt = mqtt
        self.millis = millis
        try:
            #endless loop of reading in data
            while not STOP.is_set():
                self.broadcast_nmea()
                await asyncio.sleep(millis/1000)
        
        except Exception as ex:
            print(ex)

    def broadcast_nmea(self):
        data = json.loads(json.dumps(self.current))
        if 'utcTimestamp' in data:
            nmea_zda = self.gen_zda(data)
            self.mqtt.publish('NMEA/GNZDA', nmea_zda)
            self.sock.sendto(bytes(nmea_zda, 'utf-8'), ("255.255.255.255", 50001))
            
            nmea_gga = self.gen_gga(data)
            self.mqtt.publish('NMEA/GNGGA', nmea_gga)
            self.sock.sendto(bytes(nmea_gga, 'utf-8'), ("255.255.255.255", 50001))

    def gen_zda(self, data):
    
        #$--ZDA,hhmmss.ss,dd,mm,yyyy,hh,mm *hh <CR><LF>
        #UTC From Nano To hhmmss.ss TimestampNanoseconds= 1627458044749000000

        date_time = datetime.datetime.strptime(data['utcTimestamp']['value'].split('.')[0], "%Y-%m-%d %H:%M:%S")
        timehhmmss = date_time.strftime("%H%M%S")
        year=date_time.strftime("%Y")
        month=date_time.strftime("%m")
        day=date_time.strftime("%d")
        hhmmss_ss = f"{timehhmmss}.00"
        Local_zone_hour = date_time.strftime('%H')
        Local_zone_Minutes = date_time.strftime('%M')
        
        str = 'GNZDA,%s,%s,%s,%s,%s,%s' % (hhmmss_ss, day, month, year, Local_zone_hour, Local_zone_Minutes)
        crc = 0
        for c in str:
            crc = crc ^ ord(c)
        crc = crc & 0xFF
        
        return '$%s*%0.2X\r\n' % (str, crc)

    
    def gen_gga(self, data):
        #hhmmssss = '%02d%02d%02d%s' % (time_t.tm_hour, time_t.tm_min, time_t.tm_sec, '.%02d' if 0 != 0 else '')
        #UTC From Nano To hhmmss.ss TimestampNanoseconds= 1627458044749000000
        #
        # latitudeDD, lat_pole, longitudeDD, lng_pole, fix_quality, num_sats, hdop, alt_m, heightAE, heightAMSL, dgps_age_sec, dgps_ref_id

        date_time = datetime.datetime.strptime(data['utcTimestamp']['value'].split('.')[0], "%Y-%m-%d %H:%M:%S")
        timehhmmss = date_time.strftime("%H%M%S")
        hhmmss_ss = f"{timehhmmss}.00"
        
        #Latitude Format llmm_mm
        lat_abs = abs(float(data['latitude']['value']))
        lat_deg = int(lat_abs)
        mm_mm= Decimal((lat_abs-lat_deg)*60).quantize(Decimal('0.01'))
        llmm_mm= (lat_deg*100)+mm_mm
        lat_pole_prime = 'S' if float(data['latitude']['value']) < 0 else 'N'
        lat_format = '%.2f' % (llmm_mm)
        
        #Longitude Format LLLmm_mm
        lng_abs = abs(float(data['longitude']['value']))
        lng_deg = int(lng_abs)
        MM_MM= Decimal((lng_abs-lng_deg)*60).quantize(Decimal('0.01'))
        Minuti=int(MM_MM)
        MM_MM_float=int((MM_MM-Minuti)*100)
        lng_pole_prime = 'W' if float(data['longitude']['value']) < 0 else 'E'
        lng_format = '%03d%02d.%02d' % (lng_deg,Minuti,MM_MM_float)
        
        geoidal_sep_m = float(data['heightAboveElipsoid']['value'])-float(data['heightAboveMSL']['value'])
        
        dgps_age_sec = 0
        dgps_ref_id = 0

        dgps_format = '%s,%s' % ('%.1f' % dgps_age_sec if dgps_age_sec is not None else '', '%04d' % dgps_ref_id if dgps_ref_id is not None else '')
        
        fix_quality = int(data['longitude']['quality'])*1 #Porcata per gestire la qualità del servizio. 0 => nessun dato 1 (dati corretti)=> 4 con fixed data
        num_sats = data.get('gnssFixSvCount', {}).get('value',0)
        hdop = data.get('horizontalDOP', {}).get('value',0)
        alt_m = 1
        
        str = 'GNGGA,%s,%s,%s,%s,%s,%d,%02d,%.1f,%.1f,M,%.1f,M,%s' % (hhmmss_ss, lat_format, lat_pole_prime, lng_format, lng_pole_prime, int(fix_quality), int(num_sats), float(hdop), float(alt_m), geoidal_sep_m, dgps_format)
        crc = 0
        for c in str:
            crc = crc ^ ord(c)
        crc = crc & 0xFF
        
        return '$%s*%0.2X\r\n' % (str, crc)


s2m = MSCL2NMEA()


def on_connect(mqtt, flags,rc, properties):
    '''subscribing to data needed by NMEA'''
    print('starting')
    mqtt.subscribe('us_imu/utcTimestamp')
    mqtt.subscribe('us_imu/longitude')
    mqtt.subscribe('us_imu/latitude')
    mqtt.subscribe('us_imu/gnssFixSvCount')
    mqtt.subscribe('us_imu/horizontalDOP')
    mqtt.subscribe('us_imu/heightAboveElipsoid')
    mqtt.subscribe('us_imu/heightAboveMSL')
    print('started')
   

async def on_message(client, topic, payload, qos, properties):
    s2m.value(topic, payload)    


async def main(broker_host):
    client = MqttClient('blucy-us-imu')
    client.on_connect = on_connect
    client.on_message = on_message
    await client.connect(broker_host)
    # 4 Hz
    await s2m.start(client, 250)
    await STOP.wait()
    await client.disconnect()

def ask_exit(*args):
    STOP.set()


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.add_signal_handler(signal.SIGINT, ask_exit)
    loop.add_signal_handler(signal.SIGTERM, ask_exit)
    loop.run_until_complete(main('127.0.0.1'))
