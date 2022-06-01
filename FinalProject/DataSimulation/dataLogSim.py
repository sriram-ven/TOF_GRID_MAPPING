from environment import Environment
from sweep import Sweeper, SweepData

sweeper = Sweeper(0,0,0,400) # x, y, theta, view range

for x in range(100):
    data = sweeper.getSweepData()
    if(data is not None):
        stringData = str(data.pos[0]) + " " + str(data.pos[1]) + " " + str(data.angle) + " " + str(data.distance) + "\n"
        print(stringData)
    sweeper.moveStateMachine()