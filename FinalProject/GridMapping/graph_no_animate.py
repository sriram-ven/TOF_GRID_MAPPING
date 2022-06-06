'''
This python script plots ping data on a graph, but not in real time.
'''

import serial
import time
import math
import numpy as np
import matplotlib.pyplot as plt
import csv



# ser = serial.Serial(port="COM9", baudrate=115200)

plt.ion()
fig, ax = plt.subplots()

xdata, ydata = [], []
sc = ax.scatter(xdata, ydata)

plt.xlim(-10000,10000)
plt.ylim(-10000,10000)
plt.draw()
plt.grid(True)

with open('log.csv', 'r') as csv_file:
    reader = csv.reader(csv_file)

    for row in reader:
        row = row[0].split(" ")
        print(row)

        x = float(row[0])
        y = float(row[1])
        theta = float(row[2]) % 360
        ping = float(row[3])
        
        dist_x = ping * math.cos(theta)
        dist_y = ping * math.sin(theta)

        # print(x, " ", y)

        xdata.append(x + dist_x)
        ydata.append(y + dist_y)

        sc.set_offsets(np.c_[xdata,ydata])
        fig.canvas.draw_idle()
        plt.pause(0.1)
