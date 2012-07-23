""" caltrace.py
    Show the commands required to calibrate a measurement of the Butterfly's
    power supply voltage.
"""
import serial
import time
# Open the serial port with 9600,8,N,1 settings
s = serial.Serial('/dev/ttyUSB0',9600,timeout=1)

""" The characters to be sent over the serial port to turn off logging.
    It turns out that logging needs to be turned all the way off for
    commands to be processed at full speed. """
sendstring = 'logreg 0'

def clearq():
    s.write('\r')
    rawdata = s.readline()

def main():
    # Disable rxchar logging
    for character in sendstring:
        s.write(character)
        time.sleep(0.1)
    s.write('\r')
    time.sleep(1) # Give it time to set up logging
    s.write('hello' + '\r')



if __name__ == '__main__':
     main()
