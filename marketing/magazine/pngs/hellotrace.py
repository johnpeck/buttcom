""" hellotrace.py
    Sends the characters "h e l l o \r" to the butterfly. 
"""
import serial
import time
# Open the serial port with 9600,8,N,1 settings
s = serial.Serial('/dev/ttyUSB0',9600,timeout=1)

# The characters to be sent over the serial port
sendstring = 'hello'

def main():
    for character in sendstring:
        s.write(character)
        time.sleep(0.1)
    s.write('\r')


if __name__ == '__main__':
     main()
