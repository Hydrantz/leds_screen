import board
import digitalio
import time

CONNECTED = True
CYCLE_LENGTH = 1e-2

try:
    COM_OUT = digitalio.DigitalInOut(board.D26)
    COM_OUT.direction = digitalio.Direction.OUTPUT
    COM_OUT.value = False

    COM_CLK = digitalio.DigitalInOut(board.D19)
    COM_CLK.direction = digitalio.Direction.OUTPUT
    COM_CLK.value = False

    print("CONNECTED")
except:
    CONNECTED = False
    print("running without controller lighting")

def reset_connection():
    transmit("blank")

def transmit(data: str):
    res = ''.join(format(ord(i), '08b') for i in (data+"@"))
    if CONNECTED:
        for chr in res:
            COM_OUT.value = int(chr)
            COM_CLK.value = True
            # print(chr, end='')
            time.sleep(CYCLE_LENGTH/2)
            COM_CLK.value = False
            time.sleep(CYCLE_LENGTH/2)
        # print("")