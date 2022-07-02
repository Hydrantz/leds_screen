import RPi.GPIO as GPIO


GPIO.setmode(GPIO.BOARD)

async_parallel_load = 17
clock_in = 27
data_in = 22
clock_enable_pin = 23

def setup():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(async_parallel_load, GPIO.OUT)
    GPIO.setup(clock_enable_pin, GPIO.OUT)
    GPIO.setup(clock_in, GPIO.OUT)
    GPIO.setup(data_in, GPIO.OUT)

setup()