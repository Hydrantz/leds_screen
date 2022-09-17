import serial

ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200 , timeout=.1)
ser.flush
print(ser.name)
while True:
    inpt = bytes(input(), 'utf-8')
    ser.write(inpt)
    print(inpt)