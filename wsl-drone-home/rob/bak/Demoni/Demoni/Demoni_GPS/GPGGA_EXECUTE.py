#GPGGA_EXECUTE
import socket
from GPGGA_Subs import *
from GPGGA_Read import *

while 1==1:
 TimestampNano,latitudeDD,longitudeDD,GNSSquality,heightAE,heightAMSL,HDOP = GPGGA_subs()
 print (gen_gga (TimestampNano, latitudeDD,'N', longitudeDD,'W', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0))

 #UDP_IP='192.168.36.180'
 #UDP_PORT = 50001
 Message =gen_gga (TimestampNano, latitudeDD,'N', longitudeDD,'W', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0)
 #MESSAGE=str(Message)
 #sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 #sock.sendto(Message.encode('utf-8'), (UDP_IP, UDP_PORT))
 #print ('ok')

 HOST = '192.168.36.180'  # The server's hostname or IP address
 PORT = 50001        # The port used by the server

 with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
  s.connect((HOST, PORT))
  s.sendall(Message.encode('utf-8'))
 #data = s.recv(1024)

 #print('Received', repr(data))
 print('connessione riuscita')
 #nc 192.168.36.180 50001 <Message