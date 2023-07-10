from time import sleep
import sys
sys.path.append('..')

from leds_screen.controller import buttons, lighting

sleep(2)
lighting.turn_buttons_color_default("udlrs")

while True:
    controls = buttons.get_controls()
    if controls[0]:
        print("u")
    if controls[1]:
        print("d")
    if controls[2]:
        print("l")
    if controls[3]:
        print("r")
    if controls[4]:
        print("s")
    sleep(.1)