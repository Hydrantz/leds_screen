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

def transmit_seven(string: str):
    if len(string) < 8:
            string += " "*(8-len(string))
    while True:
        value = string
        for chr in range(8):
            if value[chr]==".":
                value = value[:chr]+value[chr+1:]+" "
            if value[chr]==" ":
                data = ct.SEVEN_SEGMENT[11][::-1]
            else:
                data = ct.SEVEN_SEGMENT[int(value[chr])][::-1]
            if chr < len(value)-1:
                if value[chr+1] == ".":
                    data = data[:-1]+"1"
            digs = "1"*(7-chr)
            digs += "0"
            digs += "1"*chr
            data += digs
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
            time.sleep(.001)


def transmit_sixteen(value: str):
    if len(value) < 8:
        value += " "*(8-len(value))
    while True:
        for chr in range(8):
            data = ct.SIXTEEN_SEGMENT[value[chr]][::-1]
            digs = "1"*(7-chr)
            digs += "0"
            digs += "1"*chr
            data += digs
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
            time.sleep(.001)


def test(value: str):
    LATCH.value = False
    for chr in value:
        CLK.value = False
        if chr:
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

# while True:
#     transmit_sixteen(input())