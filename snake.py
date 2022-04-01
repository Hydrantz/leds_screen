from time import time, sleep
import keyboard, graphics
import copy

HEIGHT = graphics.HEIGHT
WIDTH = graphics.WIDTH

EMPTY_BOARD = [[graphics.EMPTY for y in range(HEIGHT)] for x in range(WIDTH)]

class Snake():

    ABS_SPEED = 1

    def __init__(self):
        self.snake_x = int(WIDTH/2)
        self.snake_y = int(HEIGHT/2)
        self.snake_speeds = (0, 0)
        self.snake_length = 5
        self.snake_offsets = []
        
        for chain in range(self.snake_length-1+Snake.ABS_SPEED):
            self.snake_offsets.append((0, -1))

        self.draw_snake()
        self.snake_offsets = self.snake_offsets[:-Snake.ABS_SPEED]

    def draw_snake(self):
        cur_coords = (self.snake_x, self.snake_y)
        time1 = time()
        graphics.draw_pixel(graphics.coords2led_index(*cur_coords), *graphics.WHITE)
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
        if self.snake_x > WIDTH or self.snake_x <= 0:
            return False
        if self.snake_y >= HEIGHT or self.snake_y < 0:
            return False
        
        for i in range(Snake.ABS_SPEED):
            self.snake_offsets.insert(0,(-self.snake_speeds[0], -self.snake_speeds[1]))
        self.draw_snake()
        self.snake_offsets = self.snake_offsets[:-Snake.ABS_SPEED]
        return True

def play():
    for i in range(graphics.WIDTH):
        graphics.draw_pixel(graphics.coords2led_index(i+1, 0), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(i+1, graphics.HEIGHT), *graphics.BLUE)
    for i in range(graphics.HEIGHT):
        graphics.draw_pixel(graphics.coords2led_index(1, i+1), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(graphics.WIDTH, i+1), *graphics.BLUE)
    snake = Snake()
    run = True
    while run:
        if keyboard.is_pressed("up"):
            snake.snake_speeds = (-1, 0)
        elif keyboard.is_pressed("down"):
            snake.snake_speeds = (1, 0)
        elif keyboard.is_pressed("left"):
            snake.snake_speeds = (0, 1)
        elif keyboard.is_pressed("right"):
            snake.snake_speeds = (0, -1)
        if snake.snake_speeds != (0, 0):
            run = snake.move_snake()
    snake = None

while True:
    play()
    sleep(2)
    graphics.clear_screen()