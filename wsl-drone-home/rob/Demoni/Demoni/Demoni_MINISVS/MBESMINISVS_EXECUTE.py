#WILDHUNT_EXECUTE

import socket
from MINISVS_Subs import *
from MINISVS_READ import *
from MINISVSSP_Read import *

while 1==1:
 #Subscription Module
 pressure,soundSpeed = miniSVS_subs()
 print (gen_minisvssp (soundSpeed))
 #print (gen_minisvssp(soundSpeed))
 #NMEA Manipulation
 #MINISVS_msg = gen_minisvs (pressure, soundSpeed)
 MINISVSSP_msg = gen_minisvssp (soundSpeed)
 #TCP Client 
 HOST = '10.0.1.102'  # The server's hostname or IP address
 PORT2 = 49153        # The port used by Depth
 messaggio = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

 #MINISVS UDP Client
  #s.connect((HOST, PORT2))
 messaggio.sendto(MINISVSSP_msg.encode(),(HOST, PORT2))
  
  #s.sendall(MINISVSSP_msg.encode('utf-8'))
  #print('MINISVS riuscita') 
 