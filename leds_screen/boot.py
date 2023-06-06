import subprocess
from time import sleep
from screen import graphics, screen_configuration as screen_conf
from controller import buttons, lighting as controller_lights, communication as comm

comm.reset_connection()

controller_lights.set_buttons_color_manually(0,0,255, "s")
buttons.wait_for('select')

controller_lights.buttons_clear()
controller_lights.set_buttons_color_manually(0,0,255, "u")
buttons.wait_for('up')

controller_lights.buttons_clear()
controller_lights.set_buttons_color_manually(0,0,255, "d")
buttons.wait_for('down')

controller_lights.buttons_clear()
controller_lights.set_buttons_color_manually(0,0,255, "l")
buttons.wait_for('left')

controller_lights.buttons_clear()
controller_lights.set_buttons_color_manually(0,0,255, "r")
buttons.wait_for('right')

controller_lights.buttons_clear()

for i in range (8):
    comm.transmit("s"+" "*i+"8"+" "*(8-i-1))
    comm.transmit("t"+" "*i+"#"+" "*(8-i-1))
    sleep(0.5)


subprocess.run(["sudo","python", "/home/pi/leds_screens/leds_screen/snake.py"])