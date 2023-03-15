import board
import digitalio

CONNECTED = None

try:
    COM_OUT = digitalio.DigitalInOut(board.D26)
    COM_OUT.direction = digitalio.Direction.OUTPUT

    COM_CLK = digitalio.DigitalInOut(board.D26)
    COM_CLK.direction = digitalio.Direction.OUTPUT
except:
    CONNECTED = False
    print("running without controller lighting")

def reset_connection():
    if CONNECTED:
        pass


chr = 'Hello World!@'
res = ''.join(format(ord(i), '08b') for i in chr)
print(str(res))

