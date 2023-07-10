"""
Lighting Module
----
- Manages lighting of custom-made controller
---
"""

import sys
sys.path.append('..')

from leds_screen.controller import communication as comm

Effects = {
   "blank" : 1,
   "blue" : 2,
   "yellow" : 3,
   "cyc" : 4,
   "strobe" : 5   
}

def transmit_effect(effect_name: str):
    """transmits an effect by given ID"""
    comm.transmit("e"+str(Effects[effect_name]), comm.ser_controller)

def direction(speed: tuple):
    buttons_clear()
    buttons = speed2direction(speed)
    turn_buttons_color_default(buttons)

def buttons_clear(buttons=""):
    """clears given buttons. if none are being given, clear all"""
    if buttons:
        comm.transmit("r"+buttons, comm.ser_controller)
    else:
        comm.transmit("c", comm.ser_controller)
    return

def color_int_to_string(value: int):
    s = ""
    if value < 10:
        s += "0"
    if value < 100:
        s += "0"
    return s + str(value)

def set_buttons_default_color(r: int, g: int, b: int):
    r = color_int_to_string(r)
    g = color_int_to_string(g)
    b = color_int_to_string(b)
    comm.transmit("b"+r+g+b, comm.ser_controller)

def turn_buttons_color_manually(r: int, g: int, b: int, buns: str):
    r = color_int_to_string(r)
    g = color_int_to_string(g)
    b = color_int_to_string(b)
    message = "m"+r+g+b+buns
    comm.transmit(message, comm.ser_controller)

def turn_buttons_color_default(buns: str):
    comm.transmit("d"+buns, comm.ser_controller)

def speed2direction(speed: tuple):
    if speed[0] == 0:
        if speed[1] == 1:
            return "dlr"
        return "ulr"
    if speed[0] == 1:
        return "rud"
    return "lud"
