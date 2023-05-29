from time import sleep
import random, sys
from os.path import dirname, abspath
from screen import graphics, screen_configuration as screen_conf
from controller import buttons, lighting as controller_lights, communication as comm
sys.path.append(dirname(dirname(abspath(__file__))))

# set screen dimensions according to global screen properties
HEIGHT = screen_conf.HEIGHT
WIDTH = screen_conf.WIDTH

# check which game controller is used
CONTROL_MODE = None
if len(sys.argv) > 1:
    CONTROL_MODE = sys.argv[1] # assign control mode if given as command line argument
if CONTROL_MODE == "keyboard":
    import keyboard # if keyboard is selected, import library accordingly

class Snake():
    # An object representing the snake

    def __init__(self):
        self.snake_x = int(WIDTH/2) # snake's X coordinate: starts at the middle of screen
        self.snake_y = int(HEIGHT/2) # snake's Y coordinate: starts at the middle of screen
        self.snake_speeds = (0, 0) # snake's speed vector: (X,Y). starts still
        self.snake_length = 4 # amount of nodes contained in the snake: starts at 4

        self.snake_nodes_offsets = []   # array of length snake_length-1. each element
                                        # represents a node in the snake, starting from
                                        # the second node. the value of each element is
                                        # a tuple containing the position of it's node
                                        # relative to the previous node

        self.last_node = None   # a tuple containing coordinates for the the last
                                # node in the snake (updated to previous game frame).
                                # since the graphics are not necessarily reset in
                                # each game frame, this is used to manually delete
                                # that node from screen
        
        # this creates initial snake_nodes_offsets. the snake is initially oriented
        # to the left.
        for chain in range(self.snake_length-1):
            self.snake_nodes_offsets.append((1, 0))

        self.draw_snake()

    def draw_snake(self):
        cur_coords = (self.snake_x, self.snake_y)
        coords = cur_coords
        if self.last_node:
            graphics.draw_pixel(graphics.coords2led_index(*self.last_node), *graphics.EMPTY)
        for chain in range(len(self.snake_nodes_offsets)):
            coords = coords[0] + self.snake_nodes_offsets[chain][0], coords[1] + self.snake_nodes_offsets[chain][1]
            graphics.draw_pixel(graphics.coords2led_index(*coords), *graphics.WHITE)
            if chain == self.snake_length-2:
                self.last_node = coords
        graphics.draw_pixel(graphics.coords2led_index(self.snake_x, self.snake_y), *graphics.RED)
        graphics.update_screen()

    def move_snake(self):
        self.snake_x += 1*self.snake_speeds[0]
        self.snake_y += 1*self.snake_speeds[1]
        if self.snake_x >= WIDTH or self.snake_x <= 1:
            return False
        if self.snake_y >= HEIGHT-1 or self.snake_y <= 0:
            return False
        
        for i in range(1):
            self.snake_nodes_offsets.insert(0,(-self.snake_speeds[0], -self.snake_speeds[1]))
        self.snake_nodes_offsets = self.snake_nodes_offsets[:-1]
        self.draw_snake()
        return True

    def enlarge_snake(self):
        self.snake_length += 1
        x_offset = self.snake_speeds[0]*-1
        y_offset = self.snake_speeds[1]*-1
        for i in range(1):
            self.snake_nodes_offsets.append((x_offset, y_offset))

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
            new_speeds = (0, -1)
        elif keys[1]:
            new_speeds = (0, 1)
        elif keys[2]:
            new_speeds = (-1, 0)
        elif keys[3]:
            new_speeds = (1, 0)
        if new_speeds != prev_speeds:
            controller_lights.direction(controller_lights.speed2direction(new_speeds))
            pass
        return new_speeds

def process_input(prev_speeds):
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
        snake_offset = snake.snake_nodes_offsets
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
        controller_lights.yellow()
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
    initial_speed = (-1, 0)
    controller_lights.direction(controller_lights.LEFT)
    if CONTROL_MODE == "keyboard":
        keyboard.wait("left")
    else:
        buttons.wait_for("left")
    controller_lights.blue()
    process_input(initial_speed)
    while run:
        color_frame()
        speeds = process_input(snake.snake_speeds)
        snake.snake_speeds = speeds
        isApple, appleCoords = manage_apples(isApple, appleCoords, snake)

        run = snake.move_snake()
        sleep(0.03)
    snake = None

if CONTROL_MODE: print(CONTROL_MODE)
sleep(2)
while True:
    comm.reset_connection()
    play()
    controller_lights.blank()
    sleep(2)
    graphics.clear_screen()