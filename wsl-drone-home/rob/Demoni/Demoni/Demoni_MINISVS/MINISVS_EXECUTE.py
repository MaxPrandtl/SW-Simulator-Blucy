#WILDHUNT_EXECUTE

import socket
from MINISVS_Subs import *
from MINISVS_READ import *
from MINISVSSP_Read import *

while 1==1:
 #Subscription Module
 pressure,soundSpeed = miniSVS_subs()
 print (gen_minisvs (pressure, soundSpeed))
 print (gen_minisvssp(soundSpeed))
 #NMEA Manipulation
 MINISVS_msg = gen_minisvs (pressure, soundSpeed)
 MINISVSSP_msg = gen_minisvssp (soundSpeed)
 #TCP Client 
 HOST = '192.168.36.180'  # The server's hostname or IP address
 PORT2 = 50002        # The port used by Depth
 with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
 #MINISVS Client
  s.connect((HOST, PORT2))
  s.sendall(MINISVS_msg.encode('utf-8'))
  s.sendall(MINISVSSP_msg.encode('utf-8'))
  print('MINISVS riuscita') 
 