from socket import socket, AF_INET, SOCK_DGRAM

class UDPSender:
  def __init__(self, port, host):
    self.__port = port
    self.__host = host

    self.__UDP_client_socket = socket(AF_INET, SOCK_DGRAM)

  def send(self, msg: str):
    bytes_to_send = msg.encode('utf-8')
    self.__UDP_client_socket.sendto(bytes_to_send, (self.__host, self.__port))
