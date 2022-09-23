import time
import neopixel
import sys
from os.path import dirname, abspath
sys.path.append(dirname(dirname(abspath(__file__))))

from leds_screen.screen import screen_configuration as screen_conf

mode = None
if len(sys.argv)>1:
    mode = sys.argv[1]

if mode == "controls":
    LED_NUM = 56
else:
    LED_NUM = screen_conf.LED_NUM
GPIO = screen_conf.GPIO
ORDER = screen_conf.ORDER

pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)

def get_rainbow_hue(part, whole):
    points = int(1530/whole*part)
    colors = {"r": 0, "g": 255, "b": 0}
    cur = "b"
    while points > 255:
        colors[cur] = abs(colors[cur]-255)
        points-=255
        if cur == "b":
            cur = "g"
        elif cur == "g":
            cur = "r"
        elif cur == "r":
            cur = "b"
    colors[cur] = abs(colors[cur] - points)
    return colors["r"], colors["g"], colors["b"]

def initiate_rainbow():
    for i in range(25):
        time1 = time.time()
        for led in range(LED_NUM):
            temp = get_rainbow_hue(led, LED_NUM)
            pixels[led] = temp
            pixels.show()
        time2 = time.time()
        print("FPS: "+str(1/((time2-time1)/600)))
        time.sleep(1 / 100)
        for led in range(LED_NUM):
            pixels[led] = (0,0,0)
            pixels.show()

def flicker():
    while True:
        pixels.fill((255,255,255))
        pixels.show()
        pixels.fill((0,0,0))
        pixels.show()

initiate_rainbow()