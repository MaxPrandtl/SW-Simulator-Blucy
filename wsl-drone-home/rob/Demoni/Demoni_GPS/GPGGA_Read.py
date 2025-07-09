#Written:2-11-11
# By: MaxPrandtl
# Find documentation of parameters here:
# http://aprs.gids.nl/nmea/#gga
# time_t is a `time_struct` (https://docs.python.org/2/library/time.html)
# alt_m, geoidal_sep_m are in meters
#HDOP: https://en.wikipedia.org/wiki/Dilution_of_precision_(navigation)
#Standard NMEA: $--GGA,hhmmss.ss,llmm.mm,a, LLLmm.mm,b,q,ss,y.y,x.x,M,g.g,M,a.a,zzzz*hh<CR><LF>

#Acquisizione Variabili (acquisisco dati da mosquitto e utilizzo solo la prima colonna)
#TimestampNano= mosquitto_sub -t "us_imu/utcTimestampNanoseconds" | cut -d ' ' -f 1 
#latitudeDD=  mosquitto_sub -t "us_imu/latitude" | cut -d ' ' -f 1
#longitudeDD=  mosquitto_sub -t "us_imu/longitude" | cut -d ' ' -f 1
#GNSSquality=  mosquitto_sub -t "us_imu/utcTimestampFlags" | cut -d ' ' -f 1
#heightAE=  mosquitto_sub -t "us_imu/heightAboveEllipsoid" | cut -d ' ' -f 1
#heightAMSL=  mosquitto_sub -t "us_imu/heightAboveMSL" | cut -d ' ' -f 1

import time
from decimal import *
from math import floor
from datetime import *

def gen_gga(TimestampNanoseconds, latitudeDD, lat_pole, longitudeDD, lng_pole, fix_quality, num_sats, hdop, alt_m, heightAE, heightAMSL, dgps_age_sec, dgps_ref_id):
  #hhmmssss = '%02d%02d%02d%s' % (time_t.tm_hour, time_t.tm_min, time_t.tm_sec, '.%02d' if 0 != 0 else '')
  #UTC From Nano To hhmmss.ss TimestampNanoseconds= 1627458044749000000

  TimestampSeconds=(int(TimestampNanoseconds) / 1000000000) #NanoSeconds 1586944173.957986
  Timestamp_ss_int=(int(int(int(TimestampNanoseconds) % 1e9)/1e7)) 
  date_time = datetime.fromtimestamp(TimestampSeconds)
  timehhmmss = date_time.strftime("%H%M%S")
 # Timestamp_ss_int_str= str(Timestamp_ss_int)
 # hhmmss_ss= timehhmmss+"."+Timestamp_ss_int_str
  if Timestamp_ss_int == 0: 
    hhmmss_ss = f"{timehhmmss}.00"
  else:
    hhmmss_ss = f"{timehhmmss}.{Timestamp_ss_int}"
  

  #Latitude Format llmm_mm
  lat_abs = abs(float(latitudeDD))
  lat_deg = int(lat_abs)
  mm_mm= Decimal((lat_abs-lat_deg)*60).quantize(Decimal('0.01'))
  llmm_mm= (lat_deg*100)+mm_mm
  lat_pole_prime = ('S' if lat_pole == 'N' else 'N') if float(latitudeDD) < 0 else lat_pole
  lat_format = '%.2f' % (llmm_mm)
  
  #Longitude Format LLLmm_mm
  lng_abs = abs(float(longitudeDD))
  lng_deg = int(lng_abs)
  MM_MM= Decimal((lng_abs-lng_deg)*60).quantize(Decimal('0.01'))
  Minuti=int(MM_MM)
  MM_MM_float=int((MM_MM-Minuti)*100)
  lng_pole_prime = ('W' if lng_pole == 'E' else 'E') if float(longitudeDD) < 0 else lng_pole
  lng_format = '%03d%02d.%02d' % (lng_deg,Minuti,MM_MM_float)
  

  geoidal_sep_m = float(heightAE)-float(heightAMSL)
  
  dgps_format = '%s,%s' % ('%.1f' % dgps_age_sec if dgps_age_sec is not None else '', '%04d' % dgps_ref_id if dgps_ref_id is not None else '')
  
  str = 'GPGGA,%s,%s,%s,%s,%s,%d,%02d,%.1f,%.1f,M,%.1f,M,%s' % (hhmmss_ss, lat_format, lat_pole_prime, lng_format, lng_pole_prime, int(fix_quality), int(num_sats), float(hdop), float(alt_m), geoidal_sep_m, dgps_format)
  crc = 0
  for c in str:
    crc = crc ^ ord(c)
  crc = crc & 0xFF
  
  return '$%s*%0.2X\r\n' % (str, crc)

#
#print (gen_gga(TimestampNano_mio, latitudeDD_data,'N', longitudeDD,'E', GNSSquality, 7, HDOP, alt_m, heightAE, heightAMSL, 0, 0))