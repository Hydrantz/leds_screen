"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

from controller import communication as comm


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

def flash():
    if CONNECTED:
        ser.write(b's')

reset_connection()