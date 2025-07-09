# from SubscriberLib import SubtopicCollection
from SubscriberLib.Topic import Topic
from Sender import TCPSender


miniSVS_sender = TCPSender(port=50002)

# Per il main2
def execute(topic: Topic):

  if topic.same_timestamp():
    print('miniSVS ok')
    pressure = float(topic['pressure'].value)
    soundSpeed = float(topic['soundSpeed'].value)

    str1 = ' %.3f %d\r\n' % (pressure, soundSpeed)
    miniSVS_sender.send(str1)
    str2 = ' %.3f' % (soundSpeed)
    miniSVS_sender.send(str2)
    print(str1, str2)
  else:
    if topic.all_valid():
      print('miniSVS diversi timestamp')
    else:
      print('Alcuni dati miniSVS non sono validi')

  # Dopo aver usato i dati resetto i valori memorizzati
  topic.reset_all()
