# MBESGPZDA_EXECUTE

import socket
from GPGGA_Subs import *
from GPZDA_Read import *
from MINISVS_Subs import *
from MINISVS_ReadQinsy import *

while 1==1:
 #Subscription Module
 TimestampNano,latitudeDD,longitudeDD,GNSSquality,heightAE,heightAMSL,HDOP = GPGGA_subs()
 pressure,soundSpeed = miniSVS_subs()
 print (gen_minisvs (soundSpeed))
 print (gen_zda(TimestampNano,'0','0',))

 #NMEA Manipulation
 GPZDA_msg = gen_zda(TimestampNano,'0','0',)
 MINISVS_msg = gen_minisvs (soundSpeed)
 #UDP Client 
 HOST = '10.0.0.99'  # The server's hostname or IP address
 PORT2 = 49153        # The port used by Depth
 messaggio = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 
 #MINISVS UDP Client
  #s.connect((HOST, PORT2))
 messaggio.sendto(GPZDA_msg.encode('utf-8'),(HOST, PORT2))
 messaggio.sendto(MINISVS_msg.encode('utf-8'),(HOST, PORT2)) 
  #s.sendall(MINISVSSP_msg.encode('utf-8'))
  #print('MINISVS riuscita') 
