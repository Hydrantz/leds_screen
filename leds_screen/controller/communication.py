import board
import digitalio
import time
import serial

CONNECTED = True
CYCLE_LENGTH = 1e-2
SERIAL_COMMUNICATION = True;

if not SERIAL_COMMUNICATION:
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
else:
    try:
        import serial
        global ser
        ser = serial.Serial(port="/dev/ttyACM0", baudrate=9600 , timeout=.1)
        ser.flush()
        CONNECTED = True
    except:
        CONNECTED = False
        print("running without controller lighting")

def reset_connection():
    if SERIAL_COMMUNICATION and CONNECTED:
        ser.flush()
    transmit("blank")

def transmit(data: str):
    res = ''.join(format(ord(i), '08b') for i in (data+"@"))
    if CONNECTED:
        if SERIAL_COMMUNICATION:
            ser.write(bytes(data+'@', 'utf-8'))
        else:
            for chr in res:
                COM_OUT.value = int(chr)
                COM_CLK.value = True
                # print(chr, end='')
                time.sleep(CYCLE_LENGTH/2)
                COM_CLK.value = False
                time.sleep(CYCLE_LENGTH/2)
            # print("")