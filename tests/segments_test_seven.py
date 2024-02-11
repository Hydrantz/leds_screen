import sys
sys.path.append('..')
from leds_screen.screen import segmented


while True:
    inpt = input()
    # segmented.transmit_score(int(inpt))
    segmented.transmit_score(inpt)