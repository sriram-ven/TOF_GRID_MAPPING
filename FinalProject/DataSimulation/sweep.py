import math
import random
from vector import Vector
from line import Line
from environment import Environment

class SweepData:
    def __init__(self, pos, angle, distance, intPoint):
        self.pos = pos
        self.angle = angle
        self.distance = distance
        self.intPoint = intPoint

class Sweeper:
    def __init__(self, x, y, dir, range):
        self.pos = Vector(x,y)
        self.range = range
        self.dir = dir
        self.environment = Environment()
        self.data = []
        self.intPoints = []

        self.moveState = 1
        self.targetDistance = 0
        self.accumulatedDistance = 0
        self.targetDirection = 0
        self.accumulatedSweep = 0
    
    def remakeEnviorment(self, amountPoints, radius, noise):
        self.environment = Environment(amountPoints, radius, noise)

    def setPos(self, x, y):
        self.pos.x = x
        self.pos.y = y

    def setRange(self, range):
        self.range = range

    def setDirection(self, angle):
        self.dir = angle

    def moveForward(self, step):
        stepVec = Vector(step * math.cos(self.dir), step * math.sin(self.dir))
        self.pos += stepVec

    def turnAmount(self, step):
        self.dir += step

    def moveStateMachine(self):
        if(self.moveState == 0):  # moving forward
            moveStep = 5
            self.moveForward(moveStep)
            self.accumulatedDistance += moveStep
            if(abs(self.accumulatedDistance - self.targetDistance) < 2 * moveStep):
                print("starting sweep")
                self.moveState = 1
                self.accumulatedSweep = 0
        elif(self.moveState == 1):  # 360 sweep
            turnStep = 0.1
            self.turnAmount(turnStep)
            self.accumulatedSweep += turnStep
            if(self.accumulatedSweep  > 2 * math.pi):
                print("starting turn")
                self.moveState = 2
                self.targetDirection = self.dir + random.random() * math.pi * 2
        else:  # turn towards new spot
            angleDiff = self.dir - self.targetDirection
            print(self.dir, self.targetDirection, angleDiff)
            turnStep = 0.1
            if(angleDiff > 0):
                self.turnAmount(-1 * turnStep)
            else:
                self.turnAmount(turnStep)
            if(abs(angleDiff) <= turnStep):
                self.moveState = 0
                self.targetDistance = self.environment.radius * 0.5 * random.random()
                self.accumulatedDistance = 0

    def getSweepData(self):
        lookVec = Vector(self.range * math.cos(self.dir), self.range * math.sin(self.dir))
        lookVec += self.pos
        lookLine = Line(self.pos, lookVec)
        for line in self.environment.lines:
            intVec = line.getIntersection(lookLine)
            if(intVec is not None):
                dist = self.pos.EuclidianDistance(intVec)
                data = SweepData((self.pos.x, self.pos.y), self.dir, dist, (intVec.x, intVec.y))
                return data
        return None

    def sweep360(self, amountPoints, filename):
        f = open(filename, "a")
        angle = self.dir
        stopAngle = self.dir + 2 * math.pi
        angleStep = 2 * math.pi / amountPoints
        while(angle < stopAngle):
            lookVec = Vector(self.range * math.cos(angle), self.range * math.sin(angle))
            lookVec = lookVec + self.pos
            lookLine = Line(self.pos, lookVec)
            for line in self.environment.lines:
                intVec = line.getIntersection(lookLine)
                if(intVec is not None):
                    self.intPoints.append((intVec.x, intVec.y))
                    dist = self.pos.EuclidianDistance(intVec)
                    data = SweepData((self.pos.x, self.pos.y), angle, dist, (intVec.x, intVec.y))
                    self.data.append(data)
                    stringData = str(self.pos.x) + " " + str(self.pos.y) + " " + str(angle) + " " + str(dist) + "\n"
                    f.write(stringData)
            angle += angleStep
        f.close()