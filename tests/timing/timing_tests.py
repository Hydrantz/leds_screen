import board
import neopixel
import time

LED_NUM = 600
GPIO = "Test"
ORDER = neopixel.GRB

LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB


pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)

time1 = time.time()
pixels.fill((255,255,255))
time2 = time.time()
pixels.show()
print(time2-time1)

time1 = time.time()
r, g, b, w = pixels._parse_color((100,255,255))
for i in range(300):
    pixels._set_item(i, r, g, b, w)
time2 = time.time()
pixels.show()
print(time2-time1)

time1 = time.time()
for i in range(LED_NUM):
    pixels[i] = (100,255,100)
time2 = time.time()
pixels.show()
print(time2-time1)