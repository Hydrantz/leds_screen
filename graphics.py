import board
import neopixel

LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB


# pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)