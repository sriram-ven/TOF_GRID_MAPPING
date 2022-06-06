'''
This script plots ping data on a matrix, but not in real time
Currently just sets values to 0/1, but can be changed to use bayes rule trivially
'''

import serial
import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import csv

# from https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/

def plotPixel(a, x1, y1, x2, y2, dx, dy, decide):
    # pk is initial decision making parameter
    # Note:x1&y1,x2&y2, dx&dy values are interchanged
    # and passed in plotPixel function so
    # it can handle both cases when m>1 & m<1
    pk = 2 * dy - dx
    for i in range(0,int(dx)):
        # print(x1, " ", y2)
        # checking either to decrement or increment the value
        # if we have to plot from (0,100) to (100,0)
        if (x1 < x2): 
            x1 = x1 + 1
        else: 
            x1 = x1 - 1

        if (pk < 0):
            # decision value will decide to plot
            # either  x1 or y1 in x's position
            if (decide == 0):
               # putpixel(x1, y1, RED);
                a[x1, y1] = 0
                pk = pk + 2 * dy
            else:
                #(y1,x1) is passed in xt
                #putpixel(y1, x1, YELLOW);
                a[y1, x1] = 0
                pk = pk + 2 * dy
        
        else:
            if (y1 < y2): 
                y1 = y1 + 1
            else:
                y1 = y1 - 1
            if (decide == 0):
                # putpixel(x1, y1, RED);
                a[x1, y1] = 0

            else:
                # putpixel(y1, x1, YELLOW);
                a[y1, x1] = 0

            pk = pk + 2 * dy - 2 * dx


# create container for grid data, initialize to 0.5
a = np.ones((1000,1000)) * 0.5

with open('log.csv', 'r') as csv_file:
    reader = csv.reader(csv_file)

    for row in reader:
        row = row[0].split(" ")
        print(row)

        x1 = float(row[0]) + 500
        y1 = float(row[1]) + 500
        theta = float(row[2]) % (2 * np.pi)
        ping = float(row[3])
        
        x2 = x1 + ping * math.cos(theta)
        y2 = y1 + ping * math.sin(theta)

        dx = abs(x2-x1)
        dy = abs(y2-y1)

        # check if the slope is greater or less than 1, and call the appropriate function
        if (dx > dy): 
            plotPixel(a, int(x1), int(y1), int(x2), int(y2), dx, dy, 0)
        else: 
            plotPixel(a, int(y1), int(x1), int(y2), int(x2), dy, dx, 1)


        # set the square the robot is currently in to 0, and set the square the ping is at to 1
        a[int(x1)][int(y1)] = 0
        a[int(x2)][int(y2)] = 1



plt.matshow(a)
plt.show()