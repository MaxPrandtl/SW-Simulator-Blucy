from decimal import *
from datetime import *

def gen_gga(TimestampNanoseconds, latitudeDD, longitudeDD,fix_quality, hdop, heightAE, heightAMSL):
  lat_pole = "N"
  lng_pole = "W"
  num_sats = 7
  alt_m = 0.3
  dgps_age_sec = 0
  dgps_ref_id = 0
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
  #lat_pole_prime = ('S' if lat_pole == 'N' else 'N') if float(latitudeDD) < 0 else lat_pole

  if float(latitudeDD) < 0:
    lat_pole_prime = 'S'
  else:
    lat_pole_prime = 'N'

  lat_format = '%.2f' % (llmm_mm)
  
  #Longitude Format LLLmm_mm
  lng_abs = abs(float(longitudeDD))
  lng_deg = int(lng_abs)
  MM_MM= Decimal((lng_abs-lng_deg)*60).quantize(Decimal('0.01'))
  Minuti=int(MM_MM)
  MM_MM_float=int((MM_MM-Minuti)*100)
  #lng_pole_prime = ('W' if lng_pole == 'E' else 'E') if float(longitudeDD) < 0 else lng_pole
  
  if float(longitudeDD) < 0:
    lng_pole_prime = 'W'
  else:
    lng_pole_prime = 'E'

  lng_format = '%03d%02d.%02d' % (lng_deg,Minuti,MM_MM_float)
  

  geoidal_sep_m = float(heightAE)-float(heightAMSL)
  
  dgps_format = '%s,%s' % ('%.1f' % dgps_age_sec if dgps_age_sec is not None else '', '%04d' % dgps_ref_id if dgps_ref_id is not None else '')
  
  str = 'GPGGA,%s,%s,%s,%s,%s,%d,%02d,%.1f,%.1f,M,%.1f,M,%s' % (hhmmss_ss, lat_format, lat_pole_prime, lng_format, lng_pole_prime, int(fix_quality), int(num_sats), float(hdop), float(alt_m), geoidal_sep_m, dgps_format)
  crc = 0
  for c in str:
    crc = crc ^ ord(c)
  crc = crc & 0xFF
  
  return '$%s*%0.2X\r\n' % (str, crc)
