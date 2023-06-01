import subprocess
from time import sleep
from screen import graphics, screen_configuration as screen_conf
from controller import buttons, lighting as controller_lights, communication as comm

comm.reset_connection()

controller_lights.direction(1)
buttons.wait_for('select')

controller_lights.direction(2)
buttons.wait_for('up')

controller_lights.direction(3)
buttons.wait_for('down')

controller_lights.direction(4)
buttons.wait_for('left')

controller_lights.direction(5)
buttons.wait_for('right')

controller_lights.direction(1)

for i in range (8):
    comm.transmit("s"+" "*i+"8"+" "*(8-i-1))
    comm.transmit("t"+" "*i+"#"+" "*(8-i-1))
    sleep(1)


subprocess.run(["sudo","python", "/home/pi/leds_screens/leds_screen/snake.py"])
