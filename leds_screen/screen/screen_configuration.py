"""
Screen Configuration
----
physical properties of the screen
"""

import neopixel, board

# physical properties of the screen:
WIDTH = 30
"""number of LED columns"""
HEIGHT = 20 
"""number of LEDs in each column"""
LED_NUM = WIDTH * HEIGHT
"""overall number of LEDs"""
ORDER = neopixel.GRB
"""order of colors"""
GPIO = board.D21
"""id of Raspberry Pi GPIO pin (PWM) the screen is connected to"""
