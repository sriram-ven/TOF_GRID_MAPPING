# Equations form https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

import math
from vector import Vector

class Line:
    def __init__(self, v1, v2):
        self.p1 = v1
        self.p2 = v2
        self.diff = v2 - v1

        self.length = math.sqrt(math.pow(self.diff.x, 2) + math.pow(self.diff.y, 2))

    def isParallel(self, l2):
        r = self.diff
        s = l2.diff
        q = l2.p1
        p = self.p1

        if(r.crossProduct(s) != 0):
            return False

        if((q - p).crossProduct(r) == 0):
            return False

        return True

    def isColinear(self, l2):
        r = self.diff
        s = l2.diff
        q = l2.p1
        p = self.p1

        if(r.crossProduct(s) != 0):
            return False

        if((q - p).crossProduct(r) != 0):
            return False

        return True

    def isIntersecting(self, l2):
        r = self.diff
        s = l2.diff
        if(r.crossProduct(s) == 0):
            return False
        
        q = l2.p1
        p = self.p1

        t = ((q - p).crossProduct(s)) / (r.crossProduct(s))
        u = ((q - p).crossProduct(r)) / (r.crossProduct(s))

        if(t < 0 or t > 1):
            return False
        
        if(u < 0 or u > 1):
            return False
        
        return True

    def getIntersection(self, l2):
        if(not self.isIntersecting(l2)):
            return None
        
        r = self.diff
        s = l2.diff        
        q = l2.p1
        p = self.p1
        t = ((q - p).crossProduct(s)) / (r.crossProduct(s))

        return p + r*t

    def __str__(self):
        return "[" + str(self.p1) + ", " + str(self.p2) + "]"
