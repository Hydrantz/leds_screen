import subprocess
from time import sleep
from screen import graphics, screen_configuration as screen_conf
from controller import buttons, lighting as controller_lights, communication as comm

import sys
sys.path.append('..')
from tools.media_manager.media_manager import load_neopixel

import pygame.mixer as mx

mx.init(24000, -16, 2, 512)
startup = mx.Sound("audio/boot_completed.wav")
bling1 = mx.Sound("audio/dig_boot/dig_boot1.wav")
bling2 = mx.Sound("audio/dig_boot/dig_boot2.wav")
bling3 = mx.Sound("audio/dig_boot/dig_boot3.wav")
bling4 = mx.Sound("audio/dig_boot/dig_boot4.wav")
bling5 = mx.Sound("audio/dig_boot/dig_boot5.wav")
bling6 = mx.Sound("audio/dig_boot/dig_boot6.wav")
bling7 = mx.Sound("audio/dig_boot/dig_boot7.wav")
bling8 = mx.Sound("audio/dig_boot/dig_boot8.wav")

load_screen = load_neopixel("bins/boot/boot.bin")

graphics.clear_screen()
comm.reset_all_connections()

controller_lights.turn_buttons_color_manually(0,0,255, "s")
buttons.wait_for('select')

controller_lights.buttons_clear()
controller_lights.turn_buttons_color_manually(0,0,255, "u")
buttons.wait_for('up')

controller_lights.buttons_clear()
controller_lights.turn_buttons_color_manually(0,0,255, "d")
buttons.wait_for('down')

controller_lights.buttons_clear()
controller_lights.turn_buttons_color_manually(0,0,255, "l")
buttons.wait_for('left')

controller_lights.buttons_clear()
controller_lights.turn_buttons_color_manually(0,0,255, "r")
buttons.wait_for('right')

controller_lights.buttons_clear()

for i in range (8):
    eval("bling"+str(i+1)).play()
    comm.transmit("s"+" "*i+"8"+" "*(8-i-1), comm.ser_score)
    comm.transmit("t"+" "*i+"#"+" "*(8-i-1), comm.ser_score)
    sleep(0.25)
comm.transmit("t"+" ", comm.ser_score)
comm.transmit("s"+" ", comm.ser_score)
startup.play()
graphics.pixels._post_brightness_buffer = load_screen
graphics.pixels.show()

subprocess.run(["sudo","python", "/home/pi/leds_screens/leds_screen/snake.py"])