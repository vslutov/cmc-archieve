#!/usr/bin/env python
"""Odu evaluating test."""

#   sle - system of linear equations solver
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

import unittest
import doctest
import solve
import grid_helpers as grid_helpers
from grid_helpers import sub, norm

from math import exp, sin, cos, e

class TestSolution(unittest.TestCase):
    """Test solution methods."""

    def setUp(self):
        """Set up data before each test."""
        steps = 200
        self.grid = [5 * i / steps for i in xrange(steps + 1)]

        self.problem_one = (lambda x, y: (y,), (0, 1))
        self.solution_one = [(exp(x),) for x in self.grid]

        self.problem_two = (lambda x, y: (0.3 * y,), (0, 1))
        self.solution_two = [(exp(0.3 * x),) for x in self.grid]

        self.problem_three = (lambda x, u, v: (v, -u), (0, 0, 1))
        self.solution_three = [(sin(x), cos(x)) for x in self.grid]

        self.another_grid = [i / steps for i in xrange(steps + 1)]
        self.problem_four = (lambda x: (-2 * x, -2, -4 * x),
                             ((1, -1, 0), (1, 0, 1 + e)))
        self.solution_four = [[x + e ** x ** 2] for x in self.another_grid]

        some = lambda x: -x / 5 - 4/25 + e**x
        self.problem_five = (lambda x: (4, -5, x),
                             ((1, 0, some(0)), (1, 0, some(1))))
        self.solution_five = [[some(x)] for x in self.another_grid]


    def test_runge_kutta_2(self):
        """Test RK2 method evaluating."""
        solution = solve.runge_kutta_2(self.problem_one, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_one)), 0.1)
        solution = solve.runge_kutta_2(self.problem_two, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_two)), 0.1)
        solution = solve.runge_kutta_2(self.problem_three, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_three)), 0.1)


    def test_runge_kutta_4(self):
        """Test RK2 method evaluating."""
        solution = solve.runge_kutta_4(self.problem_one, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_one)), 0.001)
        solution = solve.runge_kutta_4(self.problem_two, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_two)), 0.001)
        solution = solve.runge_kutta_4(self.problem_three, self.grid)
        self.assertLessEqual(norm(sub(solution, self.solution_three)), 0.001)


    def test_boundary(self):
        """Test boundary value problem solver"""
        solution = solve.boundary(self.problem_four, self.another_grid)
        self.assertLessEqual(norm(sub(solution, self.solution_four)), 0.01)
        solution = solve.boundary(self.problem_five, self.another_grid)
        self.assertLessEqual(norm(sub(solution, self.solution_five)), 0.01)

if __name__ == '__main__':
    doctest.testmod(solve)
    doctest.testmod(grid_helpers)
    unittest.main()
