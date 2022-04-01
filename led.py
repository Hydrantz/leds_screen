import time
import board
import neopixel
import math
import random

LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB


pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)


# cur_led = -1
# R = random.randint(0, 255)
# G = random.randint(0, 255)
# B = random.randint(0, 255)
# while True:
#     if cur_led == LED_NUM-1:
#         cur_led = 0
#         R = random.randint(0, 100)
#         G = random.randint(0, 100)
#         B = random.randint(0, 100)
#     else:
#         cur_led+=1
#     pixels.fill((0, 0, 0))
#     pixels[cur_led] = (R, G, B)
#     # time.sleep(100 / 1000)


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
    for i in range(5):
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
    avg = sum(times)/len(times)
    print(avg)

def flicker():
    while True:
        pixels.fill((255,255,255))
        pixels.show()
        pixels.fill((0,0,0))
        pixels.show()


initiate_rainbow()