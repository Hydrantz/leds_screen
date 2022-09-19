"""
Graphics Module
----
- controls a rectangular array of `WS2812B` LEDs using a Raspberry Pi.
- screen will not be refreshed with new values until update_screen() is called.
---
[WS2812B datasheet](https://blog.hydrantz.com/wp-stuff/wp-content/uploads/2022/04/WS2812B.pdf)
"""

import neopixel
from screen import screen_configuration as screen_conf

# creates screen object using neopixel library
pixels = neopixel.NeoPixel(screen_conf.GPIO, screen_conf.LED_NUM, pixel_order=screen_conf.ORDER, auto_write=False)

# pre-defined colors using RGB values
WHITE = pixels._parse_color((150,150,150))
BLUE = pixels._parse_color((0,0,255))
RED = pixels._parse_color((255,0,0))
GREEN = pixels._parse_color((0,255,0))
EMPTY = pixels._parse_color((0,0,0))


def RGB_parse(r,g,b):
    """converts RGB values to RGBW"""
    return pixels._parse_color((r,g,b))


def coords2led_index(x, y):
    """Converts a pixel's coordinates into its pixels chain index

    Args:
        x (int): pixel x coordinate
        x (int): pixel y coordinate

    Returns:
        int: pixels chain index of given pixel
    """
    index = (x-1)*screen_conf.HEIGHT
    if x % 2 == 1:
        index += y
    else:
        index += screen_conf.HEIGHT-y-1
    return index

def draw_pixel(led, r, g, b, w):
    """colors a given pixel index with given RGBW values.
    RGB to RGBW conversion can be done with RGB_parse()

    Args:
        led (int): pixel chain index
        r (int): Red value
        g (int): Green value
        b (int): Blue value
        w (int): White value
    """
    pixels._set_item(led, r, g, b, w)

# def draw_pixel_old(led, r, g, b, w):
#     pixels._post_brightness_buffer[led*3:3*(led+1)] = r, g, b

# def write_grid2board(grid: list):
#     global pixels
#     width = len(grid)
#     height = len(grid[0])
#     for led in range(len(pixels)):
#         col_index = led // height
#         row_index = led % height
#         if col_index % 2 == 1:
#             row_index = height - row_index - 1
#         r = grid[col_index][row_index][0]
#         g = grid[col_index][row_index][1]
#         b = grid[col_index][row_index][2]
#         w = grid[col_index][row_index][3]
#         pixels._set_item(led, *grid[col_index][row_index])
#     pixels.show()

def clear_screen():
    """turns off every pixel and updates screen"""
    pixels.fill((0,0,0))
    pixels.show()

def update_screen():
    """updates screen with its new values"""
    pixels.show()