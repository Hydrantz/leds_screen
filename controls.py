from time import sleep
from turtle import right
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

def wait_for(key):
    while True:
        if get_key_status(key):
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

def get_controls():
    return [UP.value, DOWN.value, LEFT.value, RIGHT.value]