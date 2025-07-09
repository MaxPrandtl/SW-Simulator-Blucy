#MINI SVSSV READ
#pressure=10.170
#soundSpeed=1483.429

from decimal import *
from math import floor

def gen_minisvssp(soundSpeed):
  
  #<space><PP.PPP(FLOAT)><space><SSSSSSS(mm/s integer)><CR><LF>
  soundSpeed_val=float(soundSpeed)
  
  str = ' %.3f' % (soundSpeed_val)
  
  return '%s\r\n' % (str)
  #return '%s' % (str)
