"""Ode numerical methods."""

#   ode - ordinary differential equation solver
#   Copyright (C) 2014  vslutov
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

from __future__ import absolute_import
from __future__ import division
from __future__ import generators
from __future__ import nested_scopes
from __future__ import print_function
from __future__ import unicode_literals
from __future__ import with_statement

def runge_kutta_2(problem, grid):
    """
    Solve system of ordinary differencial equation with RK2.

    problem - inital value problem in format (function, inital_value)
    function - (x, y1, y2, ...) -> (y1', y2', ...)
    inital_value - tuple of values (x0, y10, y20, ...)

    >>> from math import e
    >>> from grid_helpers import norm, sub
    >>> steps = 200
    >>> grid = [i / steps for i in xrange(steps)]
    >>> problem = (lambda x, y: (y,), (0, 1))
    >>> numerical = runge_kutta_2(problem, grid)
    >>> analytics = [(e ** x,) for x in grid]
    >>> norm(sub(numerical, analytics)) < 0.01
    True
    """
    diff, init = problem
    func_count = len(init) - 1

    step = grid[1] - grid[0]
    length = len(grid)

    solution = list(range(func_count) for i in xrange(length))
    solution[0] = init[1:]

    for i in xrange(1, length):
        part = diff(grid[i-1], *solution[i-1])
        pred = [solution[i-1][j] + part[j] * step for j in xrange(func_count)]
        part = diff(grid[i-1], *pred)
        corr = [solution[i-1][j] + part[j] * step for j in xrange(func_count)]
        solution[i] = [0.5 * (pred[j] + corr[j]) for j in xrange(func_count)]

    return solution


def runge_kutta_4(problem, grid):
    """
    Solve system of ordinary differencial equation with RK4.

    problem - inital value problem in format (function, inital_value)
    function - (x, y1, y2, ...) -> (y1', y2', ...)
    inital_value - tuple of values (x0, y10, y20, ...)

    >>> from math import e
    >>> from grid_helpers import norm, sub
    >>> steps = 200
    >>> grid = [i / steps for i in xrange(steps)]
    >>> problem = (lambda x, y: (y,), (0, 1))
    >>> numerical = runge_kutta_4(problem, grid)
    >>> analytics = [(e ** x,) for x in grid]
    >>> norm(sub(numerical, analytics)) < 0.001
    True
    """
    diff, init = problem
    func_count = len(init) - 1

    step = grid[1] - grid[0]
    length = len(grid)

    solution = list(range(func_count) for i in xrange(length))
    solution[0] = init[1:]

    for i in xrange(1, length):
        k1 = diff(grid[i-1], *solution[i-1])
        k2 = diff(grid[i-1] + step * 0.5,
                  *(solution[i-1][j] + 0.5 * k1[j] * step
                    for j in xrange(func_count)))
        k3 = diff(grid[i-1] + step * 0.5,
                  *(solution[i-1][j] + 0.5 * k2[j] * step
                    for j in xrange(func_count)))
        k4 = diff(grid[i-1] + step,
                  *(solution[i-1][j] + k3[j] * step
                    for j in xrange(func_count)))
        solution[i] = [solution[i-1][j] +
                       step/6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j])
                       for j in xrange(func_count)]

    return solution


def boundary(problem, grid):
    """
    Solve boundary value problem.

    >>> from math import e
    >>> from grid_helpers import norm, sub
    >>> grid = [i / 200 for i in xrange(201)]
    >>> answer = boundary((lambda x: (-2 * x, -2, -4 * x),
    ...                    ((1, -1, 0), (1, 0, 1 + e))), grid)
    >>> solution = [[x + e ** x ** 2] for x in grid]
    >>> norm(sub(solution, answer)) <= 0.01
    True
    """
    coeff, bound = problem

    step = grid[1] - grid[0]
    length = len(grid)

    a, b = range(length), range(length)

    A, B, C = bound[0]
    a[1] = -B / (A * step - B)
    b[1] = C * step / (A * step - B)

    for i in xrange(1, length - 1):
        co = coeff(grid[i])
        p = 1/step**2 - co[0]/(2*step)
        q = co[1] - 2/step**2
        r = 1/step**2 + co[0]/(2*step)
        s = co[2]

        a[i + 1] = -r / (p * a[i] + q)
        b[i + 1] = (s - p * b[i]) / (p * a[i] + q)

    solution = [[0] for i in xrange(length)]

    A, B, C = bound[1]
    solution[length - 1][0] = ((B * b[length-1] + C * step) /
                            (A * step + B * (1 - a[length-1])))

    for i in xrange(length-1, 0, -1):
        solution[i-1][0] = a[i] * solution[i][0] + b[i]

    return solution


if __name__ == '__main__':
    import doctest
    doctest.testmod()
