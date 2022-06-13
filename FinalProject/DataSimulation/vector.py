import math

class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def crossProduct(self, vec1):
        return (self.x * vec1.y - self.y * vec1.x)

    def getAngle(self):
        return math.atan(self.y / self.x)

    def EuclidianDistance(self, vec1):
        return math.sqrt(math.pow(self.x - vec1.x, 2) + math.pow(self.y - vec1.y, 2))

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scale):
        return Vector(self.x * scale, self.y * scale)

    def __truediv__(self, scale):
        return Vector(self.x / scale, self.y / scale)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self

    def __neg__(self):
        return Vector(-self.x, -self.y)

    def __eq__(self, other):
        return (self.x == other.x) and (self.y == other.y)

    def __ne__(self, other):
        return not((self.x == other.x) and (self.y == other.y))

    def __str__(self):
        return "(" + str(self.x) + ", " + str(self.y) + ")"