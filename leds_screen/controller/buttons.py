"""
Buttons Module
----
- Manages buttons of custom-made controller
---
"""

import board
import digitalio


UP = digitalio.DigitalInOut(board.D1)
UP.direction = digitalio.Direction.INPUT
UP.pull = digitalio.Pull.DOWN

DOWN = digitalio.DigitalInOut(board.D7)
DOWN.direction = digitalio.Direction.INPUT
DOWN.pull = digitalio.Pull.DOWN

LEFT = digitalio.DigitalInOut(board.D8)
LEFT.direction = digitalio.Direction.INPUT
LEFT.pull = digitalio.Pull.DOWN

RIGHT = digitalio.DigitalInOut(board.D25)
RIGHT.direction = digitalio.Direction.INPUT
RIGHT.pull = digitalio.Pull.DOWN

SELECT = digitalio.DigitalInOut(board.D24)
SELECT.direction = digitalio.Direction.INPUT
SELECT.pull = digitalio.Pull.DOWN

def wait_for(key):
    while True:
        status = get_key_status(key)
        if status:
            return

def get_key_status(key: str):
    if key == 'up':
        return UP.value
    elif key == 'down':
        return DOWN.value
    elif key == 'left':
        return LEFT.value
    elif key == 'right':
        return RIGHT.value
    elif key == 'select':
        return SELECT.value

def get_controls():
    return [UP.value, DOWN.value, LEFT.value, RIGHT.value, SELECT.value]
