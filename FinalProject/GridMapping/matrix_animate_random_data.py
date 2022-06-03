from re import X
import matplotlib.pyplot as plt
import numpy
import random
import math
import serial
import zmq
import time
import struct

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

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



fig,ax = plt.subplots(1,1)
a = numpy.multiply(0.5, numpy.ones((100, 100)))
im = ax.imshow(a, cmap=plt.get_cmap('Blues'), interpolation='nearest', vmin=0, vmax=1.0)

while True:       

    message = socket.recv()
    data = message.split()
    # print(data[0], data[1], data[2], data[3])
    data[0] = float(str(data[0], 'UTF-8'))
    data[1] = float(str(data[1], 'UTF-8'))
    data[2] = float(str(data[2], 'UTF-8'))
    data[3] = float(str(data[3], 'UTF-8'))

    time.sleep(1)
    socket.send(b"ack")

    x1 = data[0]
    y1 = data[1]
    theta = math.degrees(data[2]) % 360.0
    ping = data[3]

    # print(x1, y1, theta, ping)

    x2 = x1 + ping * math.cos(math.radians(theta))
    y2 = y1 + ping * math.sin(math.radians(theta))

    print(x2, y2)

    dx = abs(x2-x1)
    dy = abs(y2-y1)

    if (dx > dy): 
        plotPixel(a, int(x1), int(y1), int(x2), int(y2), dx, dy, 0)
    else: 
        plotPixel(a, int(y1), int(x1), int(y2), int(x2), dy, dx, 1)


    # # print(int(x + dist_x), " ", int(y + dist_y))
    a[int(x1)][int(y1)] = 0
    a[int(x2)][int(y2)] = 1

    im.set_data(a)
    fig.canvas.draw_idle()
    plt.pause(1)
    