from SubscriberLib.Topic import Topic
from Sender.UDPSender import UDPSender
from reader.GPGGA_read import *
from reader.GPZDA_Read import *

import sys

ip = '192.168.36.180'

us_imu_sender_GPGGA = UDPSender(port=50001, host=ip)
us_imu_sender_GPZDA = UDPSender(port=50002, host=ip)

controller_sender = UDPSender(port=8888, host=sys.argv[1])

def execute_us_imu(us_imu: Topic):

  if us_imu.same_timestamp():
    TimestampNano = us_imu['utcTimestampNanoseconds'].value
    latitudeDD = us_imu['latitude'].value
    longitudeDD = us_imu['longitude'].value
    GNSSquality = us_imu['utcTimestampFlags'].value
    heightAMSL = us_imu['heightAboveMSL'].value
    heightAE = us_imu['heightAboveEllipsoid'].value
    HDOP = us_imu['horizontalAccuracy'].value

    GPGGA_message = gen_gga(TimestampNano, latitudeDD,'N', longitudeDD,'W', GNSSquality, 7, HDOP, 0.3, heightAE, heightAMSL, 0, 0)
    GPZDA_message = gen_zda(TimestampNano, '0', '0')
    
    us_imu_sender_GPGGA.send(GPGGA_message)
    us_imu_sender_GPZDA.send(GPZDA_message)

    # print('us_imu ok')
    # print('GPGGA:', GPGGA_message)
    # print('GPZDA:', GPZDA_message)
    controller_sender.send('us_imu$0')
  else:
    if us_imu.all_valid():
      # print('us_imu diversi timestamp')
      controller_sender.send('us_imu$1')
    else:
      # print('Alcuni dati us_imu non sono validi')
      controller_sender.send('us_imu$2')

  # Dopo aver usato i dati resetto i valori memorizzati
  us_imu.reset_all()
