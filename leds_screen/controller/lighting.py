"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

CONNECTED = None

try:
    import serial
    global ser
    ser = serial.Serial(port="/dev/ttyACM0", baudrate=230400 , timeout=.1)
    ser.flush()
    CONNECTED = True
except:
    CONNECTED = False
    print("running without controller lighting")

def reset_connection():
    if CONNECTED:
        ser.flush()

def blue():
    if CONNECTED:
        ser.write(bytes("b", 'utf-8'))

def cyc():
    if CONNECTED:
        ser.write(bytes("c", 'utf-8'))

def direction(value: str):
    if CONNECTED:
        ser.write(bytes("a|"+value, 'utf-8'))

def white(value: str):
    if CONNECTED:
        ser.write(bytes("w|"+value, 'utf-8'))

reset_connection()