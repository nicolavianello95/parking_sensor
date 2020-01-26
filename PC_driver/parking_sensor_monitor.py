#########################################
#                                       #
#   file:       parking_sensor_monitor.py #
#   author:     Nicola Vianello         #
#   last edit:  Venice 25/02/2019       #
#                                       #
#########################################

import time
import serial

ADC_REF=5.1                 #ADC voltage reference
ADC_BIT=8                   #ADC bit number
SENSOR_GAIN=0.01            #sensor gain in V/Celsius
CONDITIONING_GAIN=10        #signal conditioning circuit gain

TEMPERATURE_GAIN=SENSOR_GAIN*CONDITIONING_GAIN
ADC_GAP=ADC_REF/(1<<ADC_BIT)

mySerial = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=1280000,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

mySerial.isOpen()

while 1 :
    print ('Insert "exit" to leave the program.\r\nInsert "start" to request data.')
    command = input()
    if command == 'exit':
        mySerial.close()
        exit()
    elif command == 'start':
        file = open("output.txt", 'w')
        file.write('')
        file.close()
        file = open("output.txt", 'a')
        mySerial.write(b'\0')
        for n in range(1024):
            currentTime = time.localtime(time.time()-n)
            while mySerial.inWaiting() == 0:
                pass
            data = int.from_bytes(mySerial.read(1), byteorder='big', signed=False)
            temperature = data*ADC_GAP/TEMPERATURE_GAIN
            line = '@ ' + str(currentTime.tm_hour) + ':' + str(currentTime.tm_min) + ':' + str(currentTime.tm_sec) + '\tDAC output: ' + str(data) + '\t\tTemperature: ' + str(temperature) + chr(176) + 'C'
            print (line)
            file.write(line + '\n')
        file.close()
        
