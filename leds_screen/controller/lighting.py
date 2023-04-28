"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

from controller import communication as comm

UP = 1
DOWN = 2
LEFT = 3
RIGHT = 4
ALL = 5

def speed2direction(speed: tuple):
    if speed[0] == 0:
        if speed[1] == 1:
            return DOWN
        return UP
    if speed[0] == 1:
        return RIGHT
    return LEFT

def blue():
    comm.transmit(2)

def blank():
    comm.transmit(1)

def yellow():
    comm.transmit(3)

def cyc():
    comm.transmit(4)

def direction(value: int):
    comm.transmit("a|"+str(value))

def white(value: str):
    comm.transmit("w|"+value)

def flash():
    comm.transmit(5)

comm.reset_connection()