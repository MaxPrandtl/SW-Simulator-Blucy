#MINI SVS READ
#pressure=10.170
#soundSpeed=1483.429

from decimal import *
from math import floor

def gen_minisvs(soundSpeed):
  
  #<space><PP.PPP(FLOAT)><space><SSSSSSS(mm/s integer)><CR><LF>
 #in Caso non funzionasse dividi per 10 o 100 
  soundSpeed_val=float(soundSpeed)
  
  str = ' %.3fm/sec' % (soundSpeed_val)
  
  return '%s\r\n' % (str)

#
#print (gen_minisvs(pressure,soundSpeed))
