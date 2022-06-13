from environment import Environment
from sweep import Sweeper, SweepData
import zmq
import matplotlib.pyplot as plt


context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server…")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

sweeper = Sweeper(0,0,0,400) # x, y, theta, view range

for x in range(100):
    data = sweeper.getSweepData()
    if(data is not None):
        stringData = str(data.pos[0]) + " " + str(data.pos[1]) + " " + str(data.angle) + " " + str(data.distance) + "\n"

        print("sending: ", stringData)
        socket.send_string(stringData)

        #  Get the reply.
        message = socket.recv()
        
    sweeper.moveStateMachine()

truePoints = sweeper.environment.extractPoints()
plt.plot(truePoints[0], truePoints[1])