from time import sleep, time
import random, math
import sys
from screen import graphics, screen_configuration as screen_conf, segmented
from controller import buttons, lighting as controller_lights, communication as comm
import pygame.mixer as mx


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
    """An object representing the snake"""

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

        self.score = 0
        self.state = None
        self.multiplier = 1

        # this creates initial snake_nodes_offsets. the snake is initially oriented
        # to the left.
        for _ in range(self.snake_length-1):
            self.snake_nodes_offsets.append((1, 0))

        self.draw_snake() # draw snake for the first time


    def draw_snake(self):
        """draws snake's current state to screen"""
        cur_coords = (self.snake_x, self.snake_y)
        coords = cur_coords

        # the following block remove outdated snake node from screen
        if self.last_node:
            graphics.draw_pixel(graphics.coords2led_index(*self.last_node), *graphics.EMPTY)

        # this loop iterates over snake nodes and draws them to screen
        for i, chain in enumerate(self.snake_nodes_offsets):
            coords = coords[0] + chain[0], coords[1] + chain[1] # calculate current node position
            graphics.draw_pixel(graphics.coords2led_index(*coords), *graphics.WHITE) # draw it
            # for the last node, this updates last_node
            if i == self.snake_length-2:
                self.last_node = coords

        #draws snake's head to screen
        graphics.draw_pixel(graphics.coords2led_index(self.snake_x, self.snake_y), *graphics.RED)
        graphics.update_screen() # send new data to screen


    def move_snake(self) -> bool:
        """this method moves the snake according to its current speed

        Returns:
            bool:   True - snake can move successfully
                    False - collision has happened
        """

        # update head position
        self.snake_x += self.snake_speeds[0]
        self.snake_y += self.snake_speeds[1]


        # check collision with wall
        if self.snake_x >= WIDTH or self.snake_x <= 1:
            return False
        if self.snake_y >= HEIGHT-1 or self.snake_y <= 0:
            return False

        # the following lines add the relevant offset to the top of snake_nodes_offsets.
        # also remove the bottom element since that pixel has moved.
        self.snake_nodes_offsets.insert(0,(-self.snake_speeds[0], -self.snake_speeds[1]))
        self.snake_nodes_offsets = self.snake_nodes_offsets[:-1]

        # check self collision
        node_x = self.snake_x
        node_y = self.snake_y
        for offset in self.snake_nodes_offsets:
            node_x += offset[0]
            node_y += offset[1]
            if self.snake_x == node_x and self.snake_y == node_y:
                return False

        self.draw_snake() # draw the moved snake to screen
        return True


    def enlarge_snake(self):
        """
        this method increases the snake's length by 1
        """
        self.snake_length += 1
        # these next lines calculates the offset of the new node
        x_offset = self.snake_speeds[0]*-1
        y_offset = self.snake_speeds[1]*-1
        # and adds them at the bottom of snake_nodes_offsets
        self.snake_nodes_offsets.append((x_offset, y_offset))

    # define different version of the method fir different control
    # methods

if CONTROL_MODE == "keyboard":
    def handle_input(prev_speeds) -> tuple[int, int]:
        """
        Handle player input and figure out which buttons were pressed

        Args:
            prev_speeds (int, int): snake's previous speed vector

        Returns:
            (int, int): new speed vector
        """
        new_speeds = prev_speeds
        # for each keyboard arrow, check if it is pressed and
        # set the speed vector accordingly
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

    def handle_input(prev_speeds) -> tuple[int, int]:
        """
        Handle player input and figure out which buttons were pressed

        Args:
            prev_speeds (int, int): snake's previous speed vector

        Returns:
            (int, int): new speed vector
        """
        new_speeds = prev_speeds
        keys = buttons.get_controls()
        # get an array of buttons states and check which of them
        # is being pressed
        if keys[4]:
            return new_speeds
        elif keys[0]:
            new_speeds = (0, -1)
        elif keys[1]:
            new_speeds = (0, 1)
        elif keys[2]:
            new_speeds = (-1, 0)
        elif keys[3]:
            new_speeds = (1, 0)
        if new_speeds != prev_speeds:
            # change buttons lighting if the direction has changed
            controller_lights.direction(new_speeds)
        return new_speeds


def process_input(prev_speeds):
    """
    update speed vector only if the new direction is valid.
    (not opposite from previous direction)
    otherwise, return current speed vector

    Args:
        prev_speeds (int, int): a speed vector

    Returns:
        (int, int): a speed vector
    """

    new_speeds = handle_input(prev_speeds) # get speed vector from user control

    if (prev_speeds[0] != (-1)*new_speeds[0] and
    prev_speeds[1] != (-1)*new_speeds[1]) or (
    prev_speeds == (0, 0)):
        return new_speeds
    return prev_speeds


def manage_apples(is_apple, apple_coords, snake: Snake):
    """
    generates apples and manages their state.

    Args:
        is_apple (bool): _description_
        apple_coords (_type_): _description_
        snake (Snake): _description_

    Returns:
        _type_: _description_
    """
    snake_x = snake.snake_x
    snake_y = snake.snake_y
    while not is_apple:
        apple_x = random.randint(2, WIDTH-1)
        apple_y = random.randint(1, HEIGHT-2)

        if apple_x == snake_x or apple_y == snake_y:
            continue
        
        if not snake.state:
            prob = random.randrange(10)
            if prob == 0:
                snake.state = time()
        
        snake_offset = snake.snake_nodes_offsets
        for chain in snake_offset:
            snake_x += chain[0]
            snake_y += chain[1]
            if apple_x == snake_x or apple_y == snake_y:
                continue
        apple_coords = apple_x, apple_y
        is_apple = True
        return (is_apple, apple_coords)

    apple_color = graphics.GREEN
    if snake.state:
        now = time()
        if not (now-snake.state > 12 and now-math.floor(now)>0.5):
            apple_color = graphics.YELLOW

    if apple_coords == (snake_x, snake_y):
        is_apple = False
        snake.enlarge_snake()
        if snake.state:
            if snake.multiplier < 256:
                snake.multiplier *= 2
            mx.Sound("audio/eat_mult/"+str(snake.multiplier)+".wav").play()
            segmented.transmit_text("X"+str(snake.multiplier))
        else:
            eat.play()
        controller_lights.transmit_effect("flash")
        snake.score += 10*snake.multiplier
        segmented.transmit_score(snake.score)
    else:
        graphics.draw_pixel(graphics.coords2led_index(*apple_coords), *apple_color)
    return (is_apple, apple_coords)


def color_frame():
    """
    colors the game frame
    """
    for i in range(screen_conf.WIDTH):
        graphics.draw_pixel(graphics.coords2led_index(i+1, 0), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(i+1, screen_conf.HEIGHT), *graphics.BLUE)
    for i in range(screen_conf.HEIGHT):
        graphics.draw_pixel(graphics.coords2led_index(1, i+1), *graphics.BLUE)
        graphics.draw_pixel(graphics.coords2led_index(screen_conf.WIDTH, i+1), *graphics.BLUE)


def play():
    """
    this is the game loop
    """
    is_apple = False
    apple_coords = None
    color_frame()
    snake = Snake()
    run = True
    initial_speed = (-1, 0)
    controller_lights.turn_buttons_color_default("l")
    if CONTROL_MODE == "keyboard":
        keyboard.wait("left")
    else:
        buttons.wait_for("left")
    process_input(initial_speed)
    while run:
        color_frame()
        if snake.state:
            now = time()
            if now - snake.state > 15:
                snake.state = None
                segmented.transmit_text(" ")
                snake.multiplier = 1
        speeds = process_input(snake.snake_speeds)
        snake.snake_speeds = speeds
        is_apple, apple_coords = manage_apples(is_apple, apple_coords, snake)

        run = snake.move_snake()
        # sleep(0.03)
    snake = None

# initiate game loop
mx.init(24000, -16, 2, 512)
if CONTROL_MODE:
    print(CONTROL_MODE)
sleep(2)
eat = mx.Sound("audio/eat.wav")
gameover = mx.Sound("audio/gameover.wav")
while True:
    comm.reset_all_connections()
    segmented.transmit_score(0)
    segmented.transmit_text(" ")
    play()
    gameover.play()
    controller_lights.buttons_clear()
    controller_lights.transmit_effect("blank")
    segmented.transmit_text("GAMEOVER")
    sleep(2)
    graphics.clear_screen()
    