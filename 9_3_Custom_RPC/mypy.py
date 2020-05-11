import serial

import time

serdev = '/dev/ttyACM0'

s = serial.Serial(serdev)

s.write(bytes("\r", 'UTF-8'))
time.sleep(0.5)
s.write(bytes("/LEDControl/run 2 1\r", 'UTF-8'))
time.sleep(0.5)
s.write(bytes("/LEDControl/run 1 1\r", 'UTF-8'))
time.sleep(0.5)
s.write(bytes("/LEDControl/run 3 1\r", 'UTF-8'))
time.sleep(0.5)

s.close()
