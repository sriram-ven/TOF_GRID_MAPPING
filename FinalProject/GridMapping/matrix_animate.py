# this script plots ping data on a matrix in real time used simulated data

from re import X
import matplotlib.pyplot as plt
import numpy
import random
import math
import serial

def plotPixel(a, x1, y1, x2, y2, dx, dy, decide):
    # pk is initial decision making parameter
    # Note:x1&y1,x2&y2, dx&dy values are interchanged
    # and passed in plotPixel function so
    # it can handle both cases when m>1 & m<1
    pk = 2 * dy - dx
    for i in range(0,int(dx)):
        print(x1, " ", y2)
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



fig,ax = plt.subplots(1,1)
a = numpy.multiply(0.5, numpy.ones((100, 100)))
im = ax.imshow(a, cmap=plt.get_cmap('Blues'), interpolation='nearest', vmin=0, vmax=1.0)
angle = 0
x = 50
y = 50

while True:       
    angle = (angle + 2.5) % 350

    # currLine = ser.readline()
    # currLine = currLine.decode('utf-8')
    # currArr = currLine.split(',')

    # currArr = np.array(currArr)
    # currArr = currArr.astype(float)
    currArr = [x,y,angle, 30]

    x1 = currArr[0]
    y1 = currArr[1]
    theta = currArr[2] % 360
    ping = currArr[3]
    
    x2 = x1 + ping * math.cos(math.radians(theta))
    y2 = y1 + ping * math.sin(math.radians(theta))

    dx = abs(x2-x1)
    dy = abs(y2-y1)

    if (dx > dy): 
        plotPixel(a, x1, y1, x2, y2, dx, dy, 0)
    else: 
        plotPixel(a, y1, x1, y2, x2, dy, dx, 1)


    # print(int(x + dist_x), " ", int(y + dist_y))
    a[int(x1)][int(y1)] = 0
    a[int(x2)][int(y2)] = 1

    im.set_data(a)
    fig.canvas.draw_idle()
    plt.pause(0.1)
    