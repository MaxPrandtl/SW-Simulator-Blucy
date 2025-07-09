#WILDHUNT_EXECUTE

import socket
from MINISVS_Subs import *
from MINISVS_ReadQinsy import *

while 1==1:
 #Subscription Module
 pressure,soundSpeed = miniSVS_subs()
 print (gen_minisvs (pressure, soundSpeed))
# print (gen_minisvssp(soundSpeed))
 #NMEA Manipulation
# MINISVS_msg = gen_minisvs (pressure, soundSpeed)
 MINISVSSP_msg = gen_minisvs (pressure, soundSpeed)
 #UDP Client 
 HOST = '192.168.29.234'  # The server's hostname or IP address
 PORT2 = 5602        # The port used by Depth
 messaggio = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
 #MINISVS Client UDP
 messaggio.sendto(MINISVSSP_msg.encode('utf-8'),(HOST, PORT2)) 

