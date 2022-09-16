from time import sleep
import controller.buttons, controller.lighting

sleep(2)
controller.lighting.blue()

while True:
    controls = controller.buttons.get_controls()
    if controls[0]:
        controller.lighting.direction("u")
        print("u")
    if controls[1]:
        controller.lighting.direction("d")
        print("d")
    if controls[2]:
        controller.lighting.direction("l")
        print("l")
    if controls[3]:
        controller.lighting.direction("r")
        print("r")