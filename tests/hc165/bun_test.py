from time import sleep
import RPi.GPIO as GPIO


GPIO.setmode(GPIO.BOARD)

CLK = 8
SH_LD = 10
CLK_INH = 12

def setup():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(CLK, GPIO.OUT)
    GPIO.setup(SH_LD, GPIO.OUT)
    GPIO.setup(CLK_INH, GPIO.OUT)

def clock():
    GPIO.output(CLK, 1)
    GPIO.output(CLK, 0)

setup()

GPIO.output(CLK_INH, 1)
GPIO.output(SH_LD, 1)
clock()
clock()
GPIO.output(SH_LD, 0)
clock()
GPIO.output(SH_LD, 1)
clock()
GPIO.output(CLK_INH, 0)


while True:
    input()
    clock()

# 00101100

# 10101101