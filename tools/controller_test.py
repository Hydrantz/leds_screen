from time import sleep
from leds_screen.controller import buttons, lighting

sleep(2)
lighting.blue()

while True:
    controls = buttons.get_controls()
    if controls[0]:
        lighting.direction("u")
        print("u")
    if controls[1]:
        lighting.direction("d")
        print("d")
    if controls[2]:
        lighting.direction("l")
        print("l")
    if controls[3]:
        lighting.direction("r")
        print("r")
    sleep(.1)