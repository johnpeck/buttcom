""" caltrace.py
    Show the commands required to calibrate a measurement of the Butterfly's
    power supply voltage.
"""
import serial
import time
# Open the serial port with 9600,8,N,1 settings
s = serial.Serial('/dev/ttyUSB0',9600,timeout=1)



def clearq():
    s.write('\r')


def sendcmd(sport,comstring):
    sport.write(comstring + '\r')
    time.sleep(1)

def main():
    clearq()
    # sendcmd(s,'loglevel 2')
    # By default, logging in the ISR is disabled
    raw_input('Connect the Vcc source')
    sendcmd(s,'vcounts?')
    raw_input('Connect the 0V source')
    sendcmd(s,'vcounts?')
    calval = raw_input('Calibration value? > ')
    sendcmd(s,'vslope %x' % int(calval))
    sendcmd(s,'volt?')





if __name__ == '__main__':
     main()
