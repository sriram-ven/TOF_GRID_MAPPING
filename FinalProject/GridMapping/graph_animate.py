'''
this python script uses a graph to plot ping data in real time
'''

import serial
import time
import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


ser = serial.Serial(port="COM9", baudrate=115200)

plt.ion()
fig, ax = plt.subplots()

xdata, ydata = [], []
sc = ax.scatter(xdata, ydata)

plt.xlim(-10000,10000)
plt.ylim(-10000,10000)
plt.draw()
plt.grid(True)


theta = 0
while(1):

    currLine = ser.readline()
    currLine = currLine.decode('utf-8')
    currArr = currLine.split(',')

    currArr = np.array(currArr)
    currArr = currArr.astype(float)

    x = currArr[0]
    y = currArr[1]
    theta = currArr[2] % 360
    ping = currArr[3]
    
    dist_x = ping * math.cos(math.radians(theta))
    dist_y = ping * math.sin(math.radians(theta))

    print(x, " ", y)

    xdata.append(x + dist_x)
    ydata.append(y + dist_y)

    sc.set_offsets(np.c_[xdata,ydata])
    fig.canvas.draw_idle()
    plt.pause(0.1)
    # time.sleep(1)

plt.waitforbuttonpress()

