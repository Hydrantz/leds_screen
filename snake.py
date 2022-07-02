from time import time, sleep
import keyboard, graphics
import random

HEIGHT = graphics.HEIGHT
WIDTH = graphics.WIDTH

EMPTY_BOARD = [[graphics.EMPTY for y in range(HEIGHT)] for x in range(WIDTH)]

class Snake():

    ABS_SPEED = 1

    def __init__(self):
        self.snake_x = int(WIDTH/2)
        self.snake_y = int(HEIGHT/2)
        self.snake_speeds = (0, 0)
        self.snake_length = 4
        self.snake_offsets = []
        
        for chain in range(self.snake_length-1+Snake.ABS_SPEED):
            self.snake_offsets.append((0, -1))

        self.draw_snake()
        self.snake_offsets = self.snake_offsets[:-Snake.ABS_SPEED]

    def draw_snake(self):
        cur_coords = (self.snake_x, self.snake_y)
        time1 = time()
        graphics.draw_pixel(graphics.coords2led_index(*cur_coords), *graphics.RED)
        for chain in range(len(self.snake_offsets)-Snake.ABS_SPEED):
            cur_coords = cur_coords[0] + self.snake_offsets[chain][0], cur_coords[1] + self.snake_offsets[chain][1]
            graphics.draw_pixel(graphics.coords2led_index(*cur_coords), *graphics.WHITE)
        for i in range(Snake.ABS_SPEED):
            cur_coords = cur_coords[0] + self.snake_offsets[-i-1][0], cur_coords[1] + self.snake_offsets[-i-1][1]
            graphics.draw_pixel(graphics.coords2led_index(*cur_coords), *graphics.EMPTY)
        graphics.pixels.show()
        time2 = time()
        print(1/(time2-time1))

    def move_snake(self):
        self.snake_x += Snake.ABS_SPEED*self.snake_speeds[0]
        self.snake_y += Snake.ABS_SPEED*self.snake_speeds[1]
        if self.snake_x >= WIDTH or self.snake_x <= 1:
            return False
        if self.snake_y >= HEIGHT-1 or self.snake_y <= 0:
            return False
        
        for i in range(Snake.ABS_SPEED):
            self.snake_offsets.insert(0,(-self.snake_speeds[0], -self.snake_speeds[1]))
        self.draw_snake()
        self.snake_offsets = self.snake_offsets[:-Snake.ABS_SPEED]
        return True

    def enlarge_snake(self):
        self.snake_length += Snake.ABS_SPEED
        x_offset = self.snake_speeds[0]*-1
        y_offset = self.snake_speeds[1]*-1
        for i in range(Snake.ABS_SPEED):
            self.snake_offsets.append((x_offset, y_offset))


def proccess_input(snake: Snake, new_speed):
    prev_speed = new_speed
    if keyboard.is_pressed("up"):
        new_speed = (-1, 0)
    elif keyboard.is_pressed("down"):
        new_speed = (1, 0)
    elif keyboard.is_pressed("left"):
        new_speed = (0, 1)
    elif keyboard.is_pressed("right"):
        new_speed = (0, -1)

    if (snake.snake_speeds[0] != (-1)*new_speed[0] and
    snake.snake_speeds[1] != (-1)*new_speed[1]) or (
    snake.snake_speeds == (0, 0)):
        snake.snake_speeds = new_speed
        return new_speed
    else:
        return prev_speed

def manage_apples(isApple, appleCoords, snake: Snake):
    snakeX = snake.snake_x
    snakeY = snake.snake_y
    while not isApple:
        appleX = random.randint(2, WIDTH-1)
        appleY = random.randint(1, HEIGHT-2)
        
        if appleX == snakeX or appleY == snakeY:
            continue
        snake_offset = snake.snake_offsets
        for chain in snake_offset:
            snakeX += chain[0]
            snakeY += chain[1]
            if appleX == snakeX or appleY == snakeY:
                continue
        appleCoords = appleX, appleY
        isApple = True
        graphics.draw_pixel(graphics.coords2led_index(*appleCoords), *graphics.GREEN)
        return (isApple, appleCoords)

    if appleCoords == (snakeX, snakeY):
        isApple = False
        snake.enlarge_snake()
    else:
        graphics.draw_pixel(graphics.coords2led_index(*appleCoords), *graphics.GREEN)
    return (isApple, appleCoords)


def play():
    isApple = False
    appleCoords = None
    for i in range(graphics.WIDTH):
        graphics.draw_pixel(graphics.coords2led_index(i+1, 0), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(i+1, graphics.HEIGHT), *graphics.BLUE)
    for i in range(graphics.HEIGHT):
        graphics.draw_pixel(graphics.coords2led_index(1, i+1), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(graphics.WIDTH, i+1), *graphics.BLUE)
    snake = Snake()
    run = True
    new_speed = (0, 1)
    keyboard.wait("left")

    while run:
        new_speed = proccess_input(snake, new_speed)
        isApple, appleCoords = manage_apples(isApple, appleCoords, snake)

        run = snake.move_snake()
        sleep(0.02)
    snake = None

while True:
    play()
    sleep(2)
    graphics.clear_screen()