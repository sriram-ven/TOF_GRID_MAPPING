import random
import math
from line import Line
from vector import Vector

class Environment:
    def __init__(self, amountPoints = 50, radius = 50, noise = 10):
        self.amountPoints = amountPoints
        self.radius = radius
        self.noise = noise

        self.points = []
        self.lines = []

        self.createPoints()
        self.createLines()

    def createPoints(self):
        angle = 0
        angleStep = 2 * math.pi / self.amountPoints
        while(angle < 2 * math.pi):
            randRadius = self.radius + (self.noise * (random.random() - 0.5))
            newPoint = Vector(randRadius*math.cos(angle), randRadius*math.sin(angle))
            self.points.append(newPoint)
            angle += angleStep

    def createLines(self):
        for i in range(self.amountPoints):
            newLine = Line(self.points[i - 1], self.points[i])
            self.lines.append(newLine)

    def extractPoints(self):
        lx = []
        ly = []
        for point in self.points:
            lx.append(point.x)
            ly.append(point.y)
        lx.append(self.points[0].x)
        ly.append(self.points[0].y)
        return (lx, ly)