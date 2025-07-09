import sys
import datetime
import subprocess

import os

# SYNC MCU


def _linux_set_time(timestamp):
    import ctypes
    import ctypes.util
    import time

    # /usr/include/linux/time.h:
    #
    # define CLOCK_REALTIME                     0
    CLOCK_REALTIME = 0

    # /usr/include/time.h
    #
    # struct timespec
    #  {
    #    __time_t tv_sec;            /* Seconds.  */
    #    long int tv_nsec;           /* Nanoseconds.  */
    #  };
    class timespec(ctypes.Structure):
        _fields_ = [("tv_sec", ctypes.c_long),
                    ("tv_nsec", ctypes.c_long)]

    librt = ctypes.CDLL(ctypes.util.find_library("rt"))

    ts = timespec()
    ts.tv_sec = int( time.mktime( timestamp.timetuple() ) )
    ts.tv_nsec = timestamp.timetuple()[6] * 1000000 # Millisecond to nanosecond

    # http://linux.die.net/man/3/clock_settime
    librt.clock_settime(CLOCK_REALTIME, ctypes.byref(ts))



import sys
sys.path.append('/usr/share/python3-mscl/')
from mscl import *

from decimal import *
from math import floor
from datetime import *
import asyncio
import os
import signal
import time
#TODO: change these constants to match your setup
import socket
STOP = asyncio.Event()
import datetime
import json

import csv

class MSCL2Time():
    def __init__(self,  COM_PORT = "/dev/ttyS0", bitrate = 115200):
        self.COM_PORT = COM_PORT
        self.bitrate = bitrate
        self.current = {}

    def start(self):
        #create a Serial Connection with the specified COM Port
        connection = Connection.Serial(self.COM_PORT, 115200)
        print(connection)

        #create an InertialNode with the connection
        node = InertialNode(connection)

        not_found = True 
        #endless loop of reading in data
        while (not_found):
            print(not_found)
            #get all the data packets from the node, with a timeout of 500 milliseconds
            packets = node.getDataPackets(500)
            
            for packet in packets:

                #print out the data
                #print ("Packet Received: ")
                #print(dir(packet))
                #iterate over all the data points in the packet
                for dataPoint in packet.data():

                    #print out the channel data
                    #Note: The as_string() function is being used here for simplicity. 
                    #      Other methods (ie. as_float, as_uint16, as_Vector) are also available.
                    #      To determine the format that a dataPoint is stored in, use dataPoint.storedAs().
                    #print (dataPoint.channelName() + ":", dataPoint.as_string() + " ")
                    self.current[dataPoint.channelName()] = {"value": dataPoint.as_string(), "quality": "1"}
            print(self.current.keys())
            if "utcTimestamp" in self.current and "gnssFixSvCount" in self.current:
                not_found = False
                print(self.current['utcTimestamp']['quality'])
                if self.current['utcTimestamp']['quality'] == "1":
                    return self.current['gnssFixSvCount']["value"], self.current['utcTimestamp']["value"]
                                    




if __name__ == "__main__":
    clir = MSCL2Time()
    satellites, timestamp = clir.start()
    print('GOT:', satellites, timestamp)
    tt = datetime.datetime.strptime(timestamp.split('.')[0], "%Y-%m-%d %H:%M:%S")
    tt = tt+datetime.timedelta(seconds=7200)
    print(tt)
    _linux_set_time(tt)
    
    set_datetime = subprocess.Popen(
    ['hwclock','--set', '--date', tt.isoformat()],
    stdout=subprocess.PIPE, universal_newlines=True)
    msg = set_datetime.communicate()[0].split(' ')
    print(msg)
    
    
