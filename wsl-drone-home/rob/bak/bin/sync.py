import sys
import datetime
import subprocess

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


if __name__ == "__main__":
    satellites = 0
    while satellites < 1:
        #get flags
        sub_TimestampFlags = subprocess.Popen(
        ['mosquitto_sub','-t', 'us_imu/utcTimestampFlags', '-C', '1'],
        stdout=subprocess.PIPE, universal_newlines=True)
        msg = sub_TimestampFlags.communicate()[0].replace('\n','').split(' ')[0]
        satellites = int(msg)
    sub_TimestampNano = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/utcTimestampNanoseconds', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
    msg = sub_TimestampNano.communicate()[0].split(' ')
    print(msg)
    ts = int(msg[0])

    tsm = ts/1000000000
    tt = datetime.datetime.fromtimestamp(tsm)
    print(tt)
    _linux_set_time(tt)

    
    set_datetime = subprocess.Popen(
    ['hwclock','--set', '--date', tt.isoformat()],
    stdout=subprocess.PIPE, universal_newlines=True)
    msg = set_datetime.communicate()[0].split(' ')
    print(msg)
    
    