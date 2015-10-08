from __future__ import absolute_import
from __future__ import division
from __future__ import generators
from __future__ import nested_scopes
from __future__ import print_function
from __future__ import unicode_literals
from __future__ import with_statement

import numpy as np
import matplotlib.pyplot as plt

from solve import runge_kutta_2, runge_kutta_4, boundary
import problem

def print_graphics():
    """Print nice graphics"""
    steps = 200
    grid = [5 * i / steps for i in xrange(steps)]

    numerical = runge_kutta_2(problem.FIRST, grid)
    analytical = [problem.FIRST_SOLUTION(x) for x in grid]

    first, second = zip(*runge_kutta_4(problem.SECOND, grid))

    plt.figure(figsize=(7, 4))

    plt.axes((0.15, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("Numerical solution")
    plt.plot(grid, numerical, 'k-', label='numerical')

    plt.axes((0.6, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("Analytical solution")
    plt.plot(grid, analytical, 'k-', label='analytical')

    plt.savefig("img/first_runge_kutta.png")

    plt.figure(figsize=(7, 4))

    plt.axes((0.15, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("u")
    plt.plot(grid, first, 'k-', label='u')

    plt.axes((0.6, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("v")
    plt.plot(grid, second, 'k-', label='v')

    plt.savefig("img/second_runge_kutta.png")

    steps = 20
    left, right = problem.THIRD_RANGE
    grid = [left + (right - left) * i / steps for i in xrange(steps + 1)]

    numerical = boundary(problem.THIRD, grid)
    analytical = [problem.THIRD_SOLUTION(x) for x in grid]

    plt.figure(figsize=(7, 4))

    plt.axes((0.15, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("Numerical solution")
    plt.plot(grid, numerical, 'k-', label='numerical')

    plt.axes((0.6, 0.1, 0.35, 0.8))
    plt.grid(True)
    plt.box(on=False)
    plt.title("Analytical solution")
    plt.plot(grid, analytical, 'k-', label='analytical')

    plt.savefig("img/first_boundary.png")

    steps = 20
    left, right = problem.FORTH_RANGE
    grid = [left + (right - left) * i / steps for i in xrange(steps + 1)]

    numerical = boundary(problem.FORTH, grid)

    plt.figure(figsize=(4, 4))

    plt.grid(True)
    plt.box(on=False)
    plt.title("Numerical solution")
    plt.plot(grid, numerical, 'k-', label='numerical')

    plt.savefig("img/second_boundary.png")

if __name__ == "__main__":
    print_graphics()
