# from SubscriberLib import SubtopicCollection
from SubscriberLib.Topic import Topic
#from Sender.UDPSender import UDPSender
import socket

#miniSVS_sender = UDPSender(port=50002, host='192.168.36.180')
#miniSVS_qinsy_sender = UDPSender(port=5602, host='192.186.29.111')

# Per il main2
def execute_miniSVS(topic: Topic):

  if topic.same_timestamp():
    print('miniSVS ok')
    pressure = float(topic['pressure'].value)
    soundSpeed = float(topic['soundSpeed'].value)
    pressure_dbar = pressure / 100

    str_q = ' %.5f %.3f\r\n' % (pressure_dbar, soundSpeed)
    sender_q = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sender_q.sendto(str_q.encode('utf-8'), ('192.168.29.234', 5602))


   # miniSVS_qinsy_sender.send(str_q)
   # str1 = ' %.3f %d\r\n' % (pressure, soundSpeed)
   # miniSVS_sender.send(str1)
 #   str2 = ' %.3f' % (soundSpeed)
 #   miniSVS_sender.send(str2)
    #str_q = ' %.5f %.3f\r\n' % (pressure/100, soundSpeed)
    #miniSVS_qinsy_sender.send(str(str_q))
    print(str_q)
  else:
    if topic.all_valid():
      print('miniSVS diversi timestamp')
    else:
      print('Alcuni dati miniSVS non sono validi')

  # Dopo aver usato i dati resetto i valori memorizzati
  topic.reset_all()
