
import socket
import sys

PORT = 49154
ROB_ADDRESS = '10.0.0.1' # oviamente da cambiare

TARGET_ADDRESS = '10.0.1.102' # ovviamente da cambiare

server_address = (ROB_ADDRESS, PORT)
print('starting up on {} port {}'.format(*server_address))
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(server_address)

while True:
    data, address = sock.recvfrom(4096) # questo legge qualunque udp gli arrivi su PORT
    print(data) # per debug
    if data:
        sent = sock.sendto(data, (TARGET_ADDRESS, PORT))


