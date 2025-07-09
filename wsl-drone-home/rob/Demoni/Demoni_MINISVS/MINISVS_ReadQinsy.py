#MINI SVS READ
#pressure=10.170
#soundSpeed=1483.429

from decimal import *
from math import floor

def gen_minisvs(pressure, soundSpeed):
  
  #<space><PP.PPP(FLOAT)><space><SSSSSSS(mm/s integer)><CR><LF>

  Pressure_val=float(pressure)/100
  soundSpeed_val=float(soundSpeed)
  
  str = ' %.5f %.3f' % (Pressure_val, soundSpeed_val)
  
  return '%s\r\n' % (str)

#
#print (gen_minisvs(pressure,soundSpeed))
