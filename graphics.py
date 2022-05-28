from time import sleep, time
import board
import neopixel

LED_NUM = 600
GPIO = board.D12
ORDER = neopixel.GRB

pixels = neopixel.NeoPixel(GPIO, LED_NUM, pixel_order=ORDER, auto_write=False)

HEIGHT = 20
WIDTH = 30

WHITE = pixels._parse_color((150,150,150))
BLUE = pixels._parse_color((0,0,255))
EMPTY = pixels._parse_color((0,0,0))


# pixels = [EMPTY]*LED_NUM

def coords2led_index(x, y):
    index = (x-1)*HEIGHT
    if x % 2 == 1:
        index += y
    else:
        index += HEIGHT-y-1
    return index

def draw_pixel(led, r, g, b, w):
    pixels._set_item(led, r, g, b, w)

def draw_pixel_old(led, r, g, b, w):
    pixels._post_brightness_buffer[led*3:3*(led+1)] = r, g, b

def write_grid2board(grid: list):
    global pixels
    width = len(grid)
    height = len(grid[0])
    for led in range(len(pixels)):
        col_index = led // height
        row_index = led % height
        if col_index % 2 == 1:
            row_index = height - row_index - 1
        r = grid[col_index][row_index][0]
        g = grid[col_index][row_index][1]
        b = grid[col_index][row_index][2]
        w = grid[col_index][row_index][3]
        pixels._set_item(led, *grid[col_index][row_index])
    pixels.show()

def clear_screen():
    pixels.fill((0,0,0))
    # pixels.show()


time1 = time()
draw_pixel(599, 255,0,255,0)
time2 = time()
pixels.show()
print(time2-time1)