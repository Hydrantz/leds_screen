import sys
sys.path.append('..')
from leds_screen.controller import lighting


while True:
    inpt = input()
    lighting.transmit_effect(inpt)