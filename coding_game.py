import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

# w: width of the building.
# h: height of the building.
w, h = [int(i) for i in input().split()]
n = int(input())  # maximum number of turns before game over.
x0, y0 = [int(i) for i in input().split()]

# game loop
while True:
    bomb_dir = input()  # the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)
    # Write an action using print
    # To debug: print("Debug messages...", file=sys.stderr, flush=True)
    if (bomb_dir == 'U'):
        y0 = 1
    elif (bomb_dir == 'D'):
        y0 = 1
    elif (bomb_dir == 'R'):
        x0 = 1
    elif (bomb_dir == 'L'):
        x0 = 1
    elif (bomb_dir == 'UL'):
        y0 = 1
        x0 = 1
    elif (bomb_dir == 'UR'):
        y0 = 1
        x0 = 1
        # up right
    elif (bomb_dir == 'DR'):
        y0 = 1
        x0 = 1
        # down right
    elif (bomb_dir == 'DL'):
        y0 = 1
        x0 = 1
    # the location of the next window Batman should jump to.
    print(x0, y0)
