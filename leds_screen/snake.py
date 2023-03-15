from time import sleep
import random, sys
from os.path import dirname, abspath
sys.path.append(dirname(dirname(abspath(__file__))))
from screen import graphics, screen_configuration as screen_conf
from controller import buttons, lighting as controller_lights

HEIGHT = screen_conf.HEIGHT
WIDTH = screen_conf.WIDTH

CONTROL_MODE = None
if len(sys.argv) > 1:
    CONTROL_MODE = sys.argv[1]
if CONTROL_MODE == "keyboard":
    import keyboard

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
        coords = cur_coords
        for chain in range(len(self.snake_offsets)-Snake.ABS_SPEED):
            coords = coords[0] + self.snake_offsets[chain][0], coords[1] + self.snake_offsets[chain][1]
            graphics.draw_pixel(graphics.coords2led_index(*coords), *graphics.WHITE)
        for i in range(Snake.ABS_SPEED):
            coords = coords[0] + self.snake_offsets[-i-1][0], coords[1] + self.snake_offsets[-i-1][1]
            graphics.draw_pixel(graphics.coords2led_index(*coords), *graphics.EMPTY)
        graphics.draw_pixel(graphics.coords2led_index(*cur_coords), *graphics.RED)
        graphics.update_screen()

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

if CONTROL_MODE == "keyboard":
    def handle_input(prev_speeds):
        new_speeds = prev_speeds
        if keyboard.is_pressed("up"):
            new_speeds = (-1, 0)
        elif keyboard.is_pressed("down"):
            new_speeds = (1, 0)
        elif keyboard.is_pressed("left"):
            new_speeds = (0, 1)
        elif keyboard.is_pressed("right"):
            new_speeds = (0, -1)
        return new_speeds
else:
    def handle_input(prev_speeds):
        new_speeds = prev_speeds
        keys = buttons.get_controls()
        if keys[0]:
            new_speeds = (-1, 0)
        elif keys[1]:
            new_speeds = (1, 0)
        elif keys[2]:
            new_speeds = (0, 1)
        elif keys[3]:
            new_speeds = (0, -1)
        if new_speeds != prev_speeds:
            controller_lights.direction(speed_conversion(new_speeds))
        return new_speeds

def speed_conversion(speed: tuple):
    if speed[0] == 0:
        if speed[1] == 1:
            return "l"
        return "r"
    if speed[0] == 1:
        return "d"
    return "u"

def process_input(snake: Snake, prev_speeds):
    new_speeds = handle_input(prev_speeds)

    if (prev_speeds != (-1)*new_speeds[0] and
    prev_speeds != (-1)*new_speeds[1]) or (
    prev_speeds == (0, 0)):
        return new_speeds
    return prev_speeds

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
        # controller_lights.flash()
    else:
        graphics.draw_pixel(graphics.coords2led_index(*appleCoords), *graphics.GREEN)
    return (isApple, appleCoords)

def color_frame():
    for i in range(screen_conf.WIDTH):
        graphics.draw_pixel(graphics.coords2led_index(i+1, 0), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(i+1, screen_conf.HEIGHT), *graphics.BLUE)
    for i in range(screen_conf.HEIGHT):
        graphics.draw_pixel(graphics.coords2led_index(1, i+1), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(screen_conf.WIDTH, i+1), *graphics.BLUE)


def play():
    isApple = False
    appleCoords = None
    color_frame()
    snake = Snake()
    run = True
    initial_speed = (0, 0)
    if CONTROL_MODE == "keyboard":
        keyboard.wait("left")
    else:
        buttons.wait_for("left")
    controller_lights.direction("l")
    process_input(snake, initial_speed)
    while run:
        speeds = process_input(snake, snake.snake_speeds)
        snake.snake_speeds = speeds
        isApple, appleCoords = manage_apples(isApple, appleCoords, snake)

        run = snake.move_snake()
        sleep(0.03)
    snake = None

if CONTROL_MODE: print(CONTROL_MODE)
controller_lights.reset_connection()
sleep(2)
controller_lights.cyc()
while True:
    play()
    controller_lights.reset_connection()
    sleep(2)
    graphics.clear_screen()
    color_frame()
    controller_lights.direction('a')