import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

from environment import Environment
from sweep import Sweeper, SweepData

fig = plt.figure()
ax = plt.axes(xlim=(-600, 600), ylim=(-600, 600))

xdata, ydata = [], []
dataPoints = [plt.plot([], [], 'ro')[0]]
robot = [plt.plot([], [])[0]]
patches = dataPoints + robot

sweeper = Sweeper(0,0,0,400)
truePoints = sweeper.environment.extractPoints()
plt.plot(truePoints[0], truePoints[1])

def init():
    return patches

def update(frame):
    data = sweeper.getSweepData()
    if(data is None):
        dataPoints[0].set_data(xdata, ydata)
    else:
        intPoint = data.intPoint
        xdata.append(intPoint[0])
        ydata.append(intPoint[1])
        dataPoints[0].set_data(xdata, ydata)
        f = open("log.txt", "a")
        stringData = str(data.pos[0]) + " " + str(data.pos[1]) + " " + str(data.angle) + " " + str(data.distance) + "\n"
        f.write(stringData)
        f.close()
    
    rLen = 50
    robotLineX = [sweeper.pos.x, sweeper.pos.x + rLen * np.cos(sweeper.dir)]
    robotLineY = [sweeper.pos.y, sweeper.pos.y + rLen * np.sin(sweeper.dir)]
    robot[0].set_data(robotLineX, robotLineY)

    sweeper.moveStateMachine()

    return patches

ani = FuncAnimation(fig, update, frames=None,
                    init_func=init, blit=True)
plt.show()