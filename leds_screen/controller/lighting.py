"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

from controller import communication as comm

ALL = 1
UP = 2
DOWN = 3
LEFT = 4
RIGHT = 5

def transmit_effect(effect_id: int):
    comm.transmit("e"+str(effect_id))

def speed2direction(speed: tuple):
    if speed[0] == 0:
        if speed[1] == 1:
            return DOWN
        return UP
    if speed[0] == 1:
        return RIGHT
    return LEFT

def blue():
    transmit_effect(2)

def blank():
    transmit_effect(1)

def yellow():
    transmit_effect(3)

def cyc():
    transmit_effect(4)

def direction(value):
    comm.transmit("a"+str(value))

def white(value: str):
    transmit_effect("w"+value)

def flash():
    transmit_effect(5)

comm.reset_connection()