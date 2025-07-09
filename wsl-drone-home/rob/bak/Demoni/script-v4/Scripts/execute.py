from SubscriberLib import SubtopicCollection
from SubscriberLib import TopicCollection
from reader.GPGGA_read import *
from Sender import UDPSender, TCPSender


miniSVS_sender = UDPSender(port=5606)
us_imu_sender = TCPSender(port=1234)

def execute(topic_collection: TopicCollection):
  miniSVS = topic_collection['miniSVS'] 
  us_imu = topic_collection['us_imu']

  if miniSVS.same_timestamp():
    miniSVS_sender.send(str(miniSVS))
    print('miniSVS ok')
  else:
    if miniSVS.all_valid():
      print('miniSVS diversi timestamp')
    else:
      print('Alcuni dati miniSVS non sono validi')

  if us_imu.same_timestamp():
    TimestampNano = us_imu['utcTimestampNanoseconds'].value
    latitudeDD = us_imu['latitude'].value
    longitudeDD = us_imu['longitude'].value
    GNSSquality = us_imu['utcTimestampFlags'].value
    heightAMSL = us_imu['heightAboveMSL'].value
    heightAE = us_imu['heightAboveEllipsoid'].value
    HDOP = us_imu['horizontalAccuracy'].value

    message = gen_gga(TimestampNano, latitudeDD,'N', longitudeDD,'W', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0)
    us_imu_sender.send(message)
    print('us_imu ok')
  else:
    if us_imu.all_valid():
      print('us_imu diversi timestamp')
    else:
      print('Alcuni dati us_imu non sono validi')