import board
import digitalio

CONNECTED = True

try:
    COM_OUT = digitalio.DigitalInOut(board.D26)
    COM_OUT.direction = digitalio.Direction.OUTPUT

    COM_CLK = digitalio.DigitalInOut(board.D19)
    COM_CLK.direction = digitalio.Direction.OUTPUT
    COM_CLK.value = False

except:
    CONNECTED = False
    print("running without controller lighting")

def reset_connection():
    transmit("")

def transmit(data: str):
    res = ''.join(format(ord(i), '08b') for i in (data+"@"))
    if CONNECTED:
        for chr in res:
            COM_OUT.value = int(chr)
            COM_CLK.value = True
            COM_CLK.value = False
    else:
        print(res)