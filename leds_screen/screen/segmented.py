"""
Segments Module
----
- Manages operation of segmented displays
---
"""

import sys
sys.path.append('..')

from leds_screen.controller import communication as comm

def transmit_text(text: str):
    """transmits text to segmented display"""
    comm.transmit("t"+text, comm.ser_score)

def transmit_score(score: int):
    """transmits score to segmented display"""
    data = ""
    if score == 0:
        data = " "
    else:
        data = str(score)
        length = len(data)
        if length < 8:
            data = " "*(8-length) + data
    comm.transmit("s"+data, comm.ser_score)
