import keyboard, graphics
import copy

HEIGHT = 20
WIDTH = 30
ABS_SPEED = 1
EMPTY_BOARD = [[graphics.EMPTY for y in range(HEIGHT)] for x in range(WIDTH)]

snake_x = int(WIDTH/2)
snake_y = int(HEIGHT/2)
snake_speed = (0, 0)


while True:
    if keyboard.is_pressed("up"):
        snake_speed = (1, 0)
    elif keyboard.is_pressed("down"):
        snake_speed = (-1, 0)
    elif keyboard.is_pressed("left"):
        snake_speed = (0, 1)
    elif keyboard.is_pressed("right"):
        snake_speed = (0, -1)
    snake_x += ABS_SPEED*snake_speed[0]
    if snake_x >= WIDTH or snake_x <= 0:
        snake_x = abs(snake_x - WIDTH)
    if snake_y >= HEIGHT or snake_y <= 0:
        snake_y = abs(snake_y - HEIGHT)
    
    snake_y += ABS_SPEED*snake_speed[1]
    board = EMPTY_BOARD
    board[int(snake_x)][int(snake_y)] = graphics.WHITE
    graphics.write2board(board)