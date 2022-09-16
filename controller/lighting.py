from time import sleep
import serial

ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200 , timeout=.1)
ser.flush

def blue():
    ser.write(bytes("b", 'utf-8'))

def cyc():
    ser.write(bytes("c", 'utf-8'))

def direction(value: str):
    ser.write(bytes("a|"+value, 'utf-8'))