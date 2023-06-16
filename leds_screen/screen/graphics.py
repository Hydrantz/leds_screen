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
pixels = neopixel.NeoPixel(screen_conf.GPIO, screen_conf.LED_NUM, pixel_order=screen_conf.ORDER, auto_write=False, brightness=0.5)

# pre-defined colors using RGB values
WHITE = pixels._parse_color((25,25,25))
BLUE = pixels._parse_color((0,0,75))
RED = pixels._parse_color((75,0,0))
GREEN = pixels._parse_color((0,75,0))
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
    
def draw_screen_pixel(x, y, r, g, b, w):
    """colors a given pixel index with given RGBW values.
    RGB to RGBW conversion can be done with RGB_parse()

    Args:
        led (int): pixel chain index
        r (int): Red value
        g (int): Green value
        b (int): Blue value
        w (int): White value
    """
    pixels._set_item(coords2led_index(x, y), r, g, b, w)

def clear_screen():
    """turns off every pixel and updates screen"""
    pixels.fill((0,0,0))
    pixels.show()

def update_screen():
    """updates screen with its new values"""
    pixels.show()