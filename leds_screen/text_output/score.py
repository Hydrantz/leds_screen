import threading, time
import board
import digitalio
import characters_table as ct

SER = digitalio.DigitalInOut(board.D18)
SER.direction = digitalio.Direction.OUTPUT

LATCH = digitalio.DigitalInOut(board.D15)
LATCH.direction = digitalio.Direction.OUTPUT

CLK = digitalio.DigitalInOut(board.D14)
CLK.direction = digitalio.Direction.OUTPUT

value = ""

def transmit_seven():
    while True:
        new_value = value
        length = len(new_value)
        if length < 8:
            new_value = "X"*(8-length)+new_value
        for digit in range(8):
            addup = "1"*(digit)+"0"+"1"*(8-1-digit)
            digit = new_value[digit]
            if digit == "X":
                data = "00000000"
            else:
                data = ct.SEVEN_SEGMENT[int(digit)]
            data = addup+data
            LATCH.value = False
            for seg in data:
                seg = int(seg)
                CLK.value = False
                if seg:
                    SER.value = True
                else:
                    SER.value = False
                CLK.value = True
            LATCH.value = True


def transmit_sixteen(value: str):
    data = ct.SIXTEEN_SEGMENT[value]
    LATCH.value = False
    for seg in data:
        seg = int(seg)
        CLK.value = False
        if seg:
            SER.value = True
        else:
            SER.value = False
        CLK.value = True
    LATCH.value = True


# display = threading.Thread(target=transmit)
# display.start()

# start = time.time()
# while True:
#     value = str(87654321-int(time.time()-start))
#     time.sleep(.1)

while True:
    transmit_sixteen(input())