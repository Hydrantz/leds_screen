"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

from controller import communication as comm

UP = 0;
DOWN = 1;
LEFT = 2;
RIGHT = 3;
ALL = 4

def speed2direction(speed: tuple):
    if speed[0] == 0:
        if speed[1] == 1:
            return DOWN
        return UP
    if speed[0] == 1:
        return RIGHT
    return LEFT

def blue():
    comm.transmit("b")

def blank():
    comm.transmit("blank")

def yellow():
    comm.transmit("y")

def cyc():
    comm.transmit("c")

def direction(value: int):
    comm.transmit("a|"+str(value))

def white(value: str):
    comm.transmit("w|"+value)

def flash():
    comm.transmit("s")

comm.reset_connection()