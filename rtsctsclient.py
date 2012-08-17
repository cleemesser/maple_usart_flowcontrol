# FTDI basic client
# ser.getSupportedBaudrates()
# Out[116]: 
# [('50', 50),
#  ('75', 75),
#  ('110', 110),
#  ('134', 134),
#  ('150', 150),
#  ('200', 200),
#  ('300', 300),
#  ('600', 600),
#  ('1200', 1200),
#  ('1800', 1800),
#  ('2400', 2400),
#  ('4800', 4800),
#  ('9600', 9600),
#  ('19200', 19200),
#  ('38400', 38400),
#  ('57600', 57600),
#  ('115200', 115200),
#  ('230400', 230400),
#  ('460800', 460800),
#  ('500000', 500000),
#  ('576000', 576000),
#  ('921600', 921600),
#  ('1000000', 1000000),
#  ('1152000', 1152000),
#  ('1500000', 1500000),
#  ('2000000', 2000000),
#  ('2500000', 2500000),
#  ('3000000', 3000000),
#  ('3500000', 3500000),
#  ('4000000', 4000000)]

print """
Alert: Currently rts/cts hardware handshaking is enabled
"""

import serial, glob, sys, traceback
import numpy as np
import datetime

# ls /dev/ttyUSB*  # to see what ports are available
# ser = serial.Serial(r'/dev/ttyUSB2', 9600)   # safe, works
# ser = serial.Serial(r'/dev/ttyUSB2', 921600) # works

# 9600, 115200
# BAUDRATE = 9600
BAUDRATE = 2000000
def guess_ftdi_port():
    
    # for linux, discover likely port to use; others require different methods
    port = r'/dev/ttyUSB2'
    possibleports = glob.glob(r'/dev/ttyUSB*')
    try:
        port = possibleports[0]
        return port
    except IndexError:
        print "no USB to serial port found in /dev/ttyUSB*"
        raise
print "Guess port is %s" % guess_ftdi_port()

PORT = r'/dev/ttyUSB0'
#PORT = r'/dev/ttyUSB1'
# PORT = guess_ftdi_port()
print "Opening %s at %s baud" % (PORT, BAUDRATE)
ser = serial.Serial(PORT, BAUDRATE, rtscts=True, timeout=5.0)
#ser = serial.Serial(PORT, BAUDRATE, rtscts=False, timeout=5.0)

print "Number of char in receive buffer: ", ser.inWaiting()
# can also flushInput() and flushOutput()

def hello():
    ser.write('w')
    resp = ser.readline()
    print resp
    if not resp:
        print "!!! got no response---timed out"
        


def test_256_download():
    """
    downloads 256 char which are expected to b numbers from 0..255
    and tests that this is so
    """
    ser.write('U')
    # rec = read_chars(256)
    rec = ser.read(256)
    assert rec
    uirec = [ord(ii) for ii in rec]
    assert np.all([ii == uirec[ii] for ii in range(256)])
    return uirec, rec

def read_chars(n):
    buf = []    
    for ii in xrange(n):
        c = ser.read()
        buf.append(c)
    return buf




def test_download_1024_int16():
    ser.write('d')
    resp = ser.read(size=2048) # two bytes per uint16
    assert len(resp) == 2048
    iarr= np.fromstring(resp, dtype='<i2')
    tarr = np.arange(1024, dtype=np.int16)
    assert np.all(iarr==tarr)
    return iarr, resp 


def raw_download_1024_int16():
    """
    ignore the results, get it it as fast as possible
    """
    ser.write('d')
    resp = ser.read(size=2048) # two bytes per uint16



def upload_int16arr(int16arr):
    ser.write('D')
    s = int16arr.tostring()
    ser.write(s)
    

def assemble_uints(bytearray):
    # probably there is a better way to do this ? see structure or array package
    iarr = map(ord,bytearray)
    return [ iarr[2*ii] + (iarr[2*ii+1] << 8) for ii in xrange(len(iarr)/2) ]


        
def torture_test():
    ii = 0
    start = datetime.datetime.now()
    try:
        while 1:
            a,b = test_download_1024_int16()
            ii += 1
    except (AssertionError, KeyboardInterrupt), msg:
        stop = datetime.datetime.now()
        traceback.print_exc()
        print "\nCaught exception!!!:", msg
        print "%d cycles performed %d bytes (%.1f MB) transferred before stopping" % (ii, ii*2048, ii*2048.0/1000000)
        duration = stop-start
        print "test ran for %.1f seconds" % duration.total_seconds()

