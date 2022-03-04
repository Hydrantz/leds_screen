from time import sleep
import board
import neopixel

LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB
WHITE = 255, 255, 255
EMPTY = 0, 0, 0


pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)
# pixels = [EMPTY]*LED_NUM

def write2board(grid: list):
    global pixels
    width = len(grid)
    height = len(grid[0])
    for led in range(len(pixels)):
        col_index = led // height
        row_index = led % height
        if col_index % 2 == 1:
            row_index = height - row_index - 1
        pixels[led] = grid[col_index][row_index]
    pixels.show()