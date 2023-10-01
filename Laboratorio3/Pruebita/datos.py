#!/usr/bin/python3
import serial

ser = serial.Serial(
    port='/tmp/ttyS1',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=0\
    )

# ser=serial.Serial('/tmp/ttyS1', 9600)
f = open("output.csv","w+")

print("connected to: "+ ser.portstr)

#this will store the line
line = []

while True:
    for c in ser.read():
        c=chr(c)
        print(c)
        line.append(c)
        if c== "\n:" :
            print("Line: " + "".join*(line))
            str = "".join(line)
            f.write(str)
            line=[]
            break


ser.close()

