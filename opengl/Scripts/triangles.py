from matplotlib.patches import Polygon
import matplotlib.pyplot as plt
import numpy as np
from numpy.lib.arraysetops import unique
from collections import defaultdict


class Triangle:
    def __init__(self, a, b, c, color='r'):
        self.a = a
        self.b = b
        self.c = c
        self.color = color

    def draw(self):
        X = np.array([self.a, self.b, self.c])
        plt.figure()
        plt.plot(X[:, 0], X[:, 1], self.color)
        t1 = plt.Polygon(X, color=self.color)
        plt.gca().add_patch(t1)
        plt.show()


class TriangleSet:
    def __init__(self, triangles):
        self.triangles = triangles

    def draw(self):
        plt.figure()
        for triangle in self.triangles:
            X = np.array([triangle.a, triangle.b, triangle.c])
            # plt.scatter(X[:, 0], X[:, 1], color='b')
            plt.plot(X[:, 0], X[:, 1], triangle.color)
            t1 = plt.Polygon(X, color=triangle.color)
            plt.gca().add_patch(t1)
        plt.show()

    def get_unique_points(self):
        points = {}
        i = 0
        for triangle in self.triangles:

            if triangle.a not in points:
                points[triangle.a] = i
                i += 1
            if triangle.b not in points:
                points[triangle.b] = i
                i += 1
            if triangle.c not in points:
                points[triangle.c] = i
                i += 1
        return points

    def gen_ebo(self):
        unique_points = self.get_unique_points()
        vertices = 'float vertices[] = {\n'
        for point in unique_points:
            vertices += '\t' + str(float(point[0])) + 'f, '
            vertices += str(float(point[1])) + 'f, 0.0f,\n'
        vertices += '};\n'

        index = 'unsigned int indices[] = {\n'
        for triangle in self.triangles:
            index += '\t' + str(unique_points[triangle.a]) + ', '
            index += str(unique_points[triangle.b]) + ', '
            index += str(unique_points[triangle.c]) + ',\n'
        index += '};'
        return vertices + index

    def gen_all(self):
        triangles = ''
        compilation = 'float* vertices[] = {'
        for i, triangle in enumerate(self.triangles):
            triangles += 'float v' + str(i) + '[] = {\n'
            triangles += '\t' + str(float(triangle.a[0])) + 'f, '
            triangles += str(float(triangle.a[1])) + 'f, 0.0f,\n'
            triangles += '\t' + str(float(triangle.b[0])) + 'f, '
            triangles += str(float(triangle.b[1])) + 'f, 0.0f,\n'
            triangles += '\t' + str(float(triangle.c[0])) + 'f, '
            triangles += str(float(triangle.c[1])) + 'f, 0.0f,\n'
            triangles += '};\n'
            compilation += 'v' + str(i) + ' ,'
        return triangles + compilation + '};'


A = (0.25, 0.2)
B = (-0.15, 0.2)
C = (0.55, 0.2)
D = (-0.55, 0.2)
E = (-0.15, -0.2)
F = (-0.55, 0.6)
G = (0.25, 0.6)
I = (0.25, -0.1)
K = (-0.05, -0.3)
L = (-0.25, -0.5)
M = (-0.45, -0.3)
N = (0.05, -0.3)
O = (0.45, -0.3)
P = (0.25, -0.5)
H = (-0.25, -0.1)
J = (-0.05, -0.1)


TA = [Triangle(F, D, B, 'g'),
      Triangle(F, G, B, 'b')
      ]

TB = [Triangle(D, E, A, 'r'),
      Triangle(A, J, I, 'g'),
      Triangle(A, I, C, 'y'),
      Triangle(H, M, L, 'r'),
      Triangle(H, K, L, 'c'),
      Triangle(I, N, P, 'b'),
      Triangle(I, O, P, 'r')]

print(TriangleSet(TA).gen_ebo())
print("-" * 20)
print(TriangleSet(TB).gen_ebo())
TriangleSet(TB).draw()
