import keyboard

HEIGHT = 20
WIDTH = 30

snake_x = int(WIDTH/2)
snake_y = int(HEIGHT/2)
snake_speed = (0, 0)


while True:
    print(keyboard.read_key())