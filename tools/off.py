import neopixel
from leds_screen.screen import screen_configuration as screen_conf


LED_NUM = screen_conf.LED_NUM
GPIO = screen_conf.GPIO
ORDER = screen_conf.ORDER

pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)

pixels.fill((0,0,0))
pixels.show()