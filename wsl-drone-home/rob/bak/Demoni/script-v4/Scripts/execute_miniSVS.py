from SubscriberLib.Exceptions import SubtopicNotFoundException
from SubscriberLib.Topic import Topic
from Sender.UDPSender import UDPSender
# import socket

import sys

ip_quinsy = '192.168.29.234'
port_quinsy = 5602

ip_fog = '192.168.36.180'
port_fog = 50002

miniSVS_sender = UDPSender(port=port_fog, host=ip_fog)
miniSVS_qinsy_sender = UDPSender(port=port_quinsy, host=ip_quinsy)

controller_sender = UDPSender(port=8888, host=sys.argv[1]) # host: ip computer di terra

def execute_miniSVS(miniSVS: Topic):
  
  if miniSVS.same_timestamp():
    # print('miniSVS ok')
    try:
      pressure_bar = float(miniSVS['pressure'].value)
      soundSpeed = float(miniSVS['soundSpeed'].value)
    except SubtopicNotFoundException:
      pass
    
    pressure_dbar = pressure_bar / 100

    str_q = ' %.5f %.3f\r\n' % (pressure_dbar, soundSpeed)
    miniSVS_qinsy_sender.send(str_q)

    # Fog
    str1 = ' %.3f %d\r\n' % (pressure_bar, soundSpeed)
    miniSVS_sender.send(str1)

    str2 = ' %.3f' % (soundSpeed)
    miniSVS_sender.send(str2)

    controller_sender.send('miniSVS$0')
  else:
    if miniSVS.all_valid():
      # print('miniSVS diversi timestamp')
      controller_sender.send('miniSVS$1')
    else:
      # print('Alcuni dati miniSVS non sono validi')
      controller_sender.send('miniSVS$2')

  # Dopo aver usato i dati resetto i valori memorizzati
  miniSVS.reset_all()
