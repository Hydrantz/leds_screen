"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

import serial

ser = serial.Serial(port="/dev/ttyACM0", baudrate=230400 , timeout=.1)
ser.flush

def blue():
    ser.write(bytes("b", 'utf-8'))

def cyc():
    ser.write(bytes("c", 'utf-8'))

def direction(value: str):
    ser.write(bytes("a|"+value, 'utf-8'))

def white(value: str):
    ser.write(bytes("w|"+value, 'utf-8'))