# from SubscriberLib import SubtopicCollection
from SubscriberLib.Topic import Topic
from Sender import TCPSender


us_imu_sender = TCPSender(port=50002)

# Per il main2
def execute(topic: Topic):

  if topic.same_timestamp():
    print('miniSVS ok')
    us_imu_sender.send(topic.complete_log())
  else:
    if topic.all_valid():
      print('miniSVS diversi timestamp')
    else:
      print('Alcuni dati miniSVS non sono validi')

  # Dopo aver usato i dati resetto i valori memorizzati
  topic.reset_all()