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

load_screen = load_neopixel("bins/boot/boot.bin")

subprocess.run(["sudo","python", "/home/pi/leds_screens/leds_screen/snake.py"])