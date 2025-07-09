from SubscriberLib import Topic
from SubscriberLib import Subscriber
from functions.data_parser import parser
from Scripts.execute2 import execute


miniSVS = Topic(
  'miniSVS', 
  ['pressure', 'soundSpeed'], 
  data_parser=parser, 
  on_all_received=execute
)

subscriber = Subscriber([miniSVS])

subscriber.listen()
