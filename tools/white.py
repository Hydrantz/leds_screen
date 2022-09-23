import neopixel, sys
from os.path import dirname, abspath
sys.path.append(dirname(dirname(abspath(__file__))))

from leds_screen.screen import screen_configuration as screen_conf

LED_NUM = screen_conf.LED_NUM
GPIO = screen_conf.GPIO
ORDER = screen_conf.ORDER

pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)
pixels.fill((255,255,255))
pixels.show()