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

def white(value: str):
    transmit_effect("w"+value)

def flash():
    transmit_effect(5)

def direction(value):
    comm.transmit("a"+str(value))

def buttons_clear():
    comm.transmit("a6")

def color_int_to_string(value: int):
    s = ""
    if value < 10:
        s += "0"
    if value < 100:
        s += "0"
    return s + str(value)

def buttons_default_color(r: int, g: int, b: int, sel: bool):
    r = color_int_to_string(r)
    g = color_int_to_string(g)
    b = color_int_to_string(b)
    comm.transmit("b"+r+g+b+str(int(sel)))

def set_buttons_color_manually(r: int, g: int, b: int, buns: str):
    r = color_int_to_string(r)
    g = color_int_to_string(g)
    b = color_int_to_string(b)
    message = "m"+r+g+b+buns
    comm.transmit(message)

def set_sel_state(sel: bool):
    comm.transmit("o"+str(int(sel)))
