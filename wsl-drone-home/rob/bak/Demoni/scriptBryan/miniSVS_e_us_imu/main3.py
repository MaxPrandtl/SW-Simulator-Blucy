from SubscriberLib import Topic
from SubscriberLib import Subscriber
from functions.data_parser import parser
from Scripts.execute_miniSVS import execute_miniSVS
from Scripts.execute_us_imu import execute_us_imu


miniSVS = Topic(
  'miniSVS',
  ['pressure', 'soundSpeed'],
  data_parser=parser,
  on_all_received=execute_miniSVS
)

us_imu = Topic('us_imu', [
  'utcTimestampNanoseconds',
  'latitude',
  'longitude',
  'utcTimestampFlags',
  'heightAboveEllipsoid',
  'heightAboveMSL',
  'horizontalAccuracy'],
  on_all_received=execute_us_imu,
  data_parser=parser)

subscriber = Subscriber([miniSVS])

subscriber.listen()
