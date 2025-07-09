#Written:5-11-11
# By: MaxPrandtl

#Acquisizione Variabili (acquisisco dati da mosquitto e utilizzo solo la prima colonna)
#TimestampNano= mosquitto_sub -t "us_imu/utcTimestampNanoseconds" | cut -d ' ' -f 1 
#TimestampNanoseconds= 1638458044749000000

import time
from decimal import *
from math import floor
from datetime import *

def gen_zda(TimestampNanoseconds):

  Local_zone_hour = 0
  Local_zone_Minutes = 0  
  #$--ZDA,hhmmss.ss,dd,mm,yyyy,hh,mm *hh <CR><LF>
  #UTC From Nano To hhmmss.ss TimestampNanoseconds= 1627458044749000000

  TimestampSeconds=(int(TimestampNanoseconds) / 1000000000) #NanoSeconds 1586944173.957986
  Timestamp_ss_int=(int(int(int(TimestampNanoseconds) % 1e9)/1e7)) 
  date_time = datetime.fromtimestamp(TimestampSeconds)
  timehhmmss = date_time.strftime("%H%M%S")
  year=date_time.strftime("%Y")
  month=date_time.strftime("%m")
  day=date_time.strftime("%d")
  if Timestamp_ss_int == 0: 
    hhmmss_ss = f"{timehhmmss}.00"
  else:
    hhmmss_ss = f"{timehhmmss}.{Timestamp_ss_int}"
  
  str = 'GPZDA,%s,%s,%s,%s,%s,%s' % (hhmmss_ss, day, month, year, Local_zone_hour, Local_zone_Minutes)
  crc = 0
  for c in str:
    crc = crc ^ ord(c)
  crc = crc & 0xFF
  
  return '$%s*%0.2X\r\n' % (str, crc)

#
#print (gen_zda(TimestampNanoseconds,'0','0',))
