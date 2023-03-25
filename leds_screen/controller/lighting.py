"""
Controller Module
----
- Manages lighting of custom-made controller
---
"""

from controller import communication as comm

def blue():
    comm.transmit("b")

def blank():
    comm.transmit("blank")

def yellow():
    comm.transmit("y")

def cyc():
    comm.transmit("c")

def direction(value: str):
    comm.transmit("a|"+value)

def white(value: str):
    comm.transmit("w|"+value)

def flash():
    comm.transmit("s")

comm.reset_connection()