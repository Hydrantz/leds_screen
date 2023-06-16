import neopixel, board
import media_manager

buff = media_manager.load_neopixel(input("enter .bin path\n"))

pixels = neopixel.NeoPixel(board.D21, 600, pixel_order=neopixel.GRB, auto_write=False, brightness=0.1)
pixels._post_brightness_buffer = buff
pixels.show()