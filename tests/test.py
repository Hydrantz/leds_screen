import board
import neopixel
import time


LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB


pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)
time1 = time.time()
pixels.fill((255,0,255))
time2 = time.time()
pixels.show()
time3 = time.time()
print(time2-time1)
print(time3-time2)
print("")
time1 = time.time()
pixels[30] = (0,255,255)
time2 = time.time()
pixels.show()
time3 = time.time()
print(time2-time1)
print(time3-time2)
print("")