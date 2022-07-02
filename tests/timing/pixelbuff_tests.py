import graphics
import time

test = graphics.pixels._post_brightness_buffer
i = 0 * 3
one = time.time()
graphics.pixels._post_brightness_buffer = test[i:i+3] = 0, 0, 255
two = time.time()
graphics.pixels.show()

print(two-one)