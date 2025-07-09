#WILDHUNT_EXECUTE

import socket
from GPGGA_Subs import *
from GPGGA_Read import *
from GPZDA_Read import *

while 1==1:
 #Subscription Module
 TimestampNano,latitudeDD,longitudeDD,GNSSquality,heightAE,heightAMSL,HDOP = GPGGA_subs()
 print (gen_gga (TimestampNano, latitudeDD,'N', longitudeDD,'E', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0))
 print (gen_zda(TimestampNano,'0','0',))

 #NMEA Manipulation
 GPGGA_msg = gen_gga (TimestampNano, latitudeDD,'N', longitudeDD,'E', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0)
 GPZDA_msg= gen_zda(TimestampNano,'0','0',)

 #TCP Client 
 HOST = '192.168.36.180'  # The server's hostname or IP address
 PORT1 = 50001        # The port used by GNSS1
 PORT2 = 50002        # The port used by UTC1
 with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
 #GGA Client
  s.connect((HOST, PORT1))
  s.sendall(GPGGA_msg.encode('utf-8'))
  #s.close()
  print('GPGGA riuscita') 
 #del s
 
 #with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
 #ZDA Client
 # s.connect((HOST, PORT2))
  s.sendall(GPZDA_msg.encode('utf-8'))
  #s.close()
  print('GPZDA riuscita')
 #del s