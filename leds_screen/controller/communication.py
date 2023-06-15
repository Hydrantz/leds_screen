"""
Communication Module
----
- manages serial communication between Raspberry Pi and Arduino.
- sends control commands for controller buttons, controller lights and scoreboard
---
"""

import time

CONNECTED = True
CYCLE_LENGTH = 1e-2
SERIAL_COMMUNICATION = True

try:
    import serial
    ser_controller = serial.Serial(port="/dev/ttyUSB_CONTROLLER", baudrate=9600 , timeout=.1)
    ser_score = serial.Serial(port="/dev/ttyACM_ARDUINO_UNO", baudrate=9600 , timeout=.1)
    CONNECTED = True
except ImportError:
    CONNECTED = False
    print("running without controller lighting")

def reset_connection(ser):
    if SERIAL_COMMUNICATION and CONNECTED:
        ser.flush()
    time.sleep(2)

def reset_all_connections():
    if SERIAL_COMMUNICATION and CONNECTED:
        ser_controller.flush()
        ser_score.flush()
    time.sleep(2)

def transmit(data: str, ser):
    if CONNECTED:
        ser.write(bytes(data+'@', 'utf-8'))

reset_all_connections()
