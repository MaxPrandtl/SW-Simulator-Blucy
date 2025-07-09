from socket import socket, AF_INET, SOCK_STREAM

class TCPSender:
  def __init__(self, port=3000, host='192.168.36.180') -> None:
    self.__port = port
    self.__host = host

    self.__TCP_client_socket = socket(family=AF_INET, type=SOCK_STREAM)
    try:
      self.__TCP_client_socket.connect((self.__host, self.__port))
    except ConnectionRefusedError:
      print(f'Connection error to {self.__host}:{self.__port}')


  def send(self, msg: str):
    try: 
      bytes_to_send = bytes(msg, "utf-8")
      self.__TCP_client_socket.sendall(bytes_to_send)
    except BrokenPipeError:
      print(f'TCP error: Broken pipe')