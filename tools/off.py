import board
import neopixel

LED_NUM = 600
GPIO = board.D21
ORDER = neopixel.GRB

pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)

pixels.fill((0,0,0))
pixels.show()