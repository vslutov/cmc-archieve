#!/usr/bin/env python
"""Matrix evaluating test."""

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

from __future__ import absolute_import, print_function, unicode_literals
from __future__ import division

import unittest
import doctest
import matrix_functions as mat
import gauss


class TestSolution(unittest.TestCase):

    """Test gauss solution."""

    def setUp(self):
        """Set up data before each test."""
        self.matrix_one = [[2, 2, -1, 1],
                           [4, 3, -1, 2],
                           [8, 5, -3, 4],
                           [3, 3, -2, 4]]
        self.column_one = [[4],
                           [6],
                           [12],
                           [6]]
        self.solution_one = [[3/5],
                             [1],
                             [-1],
                             [-1/5]]

        self.matrix_two = [[2, 3, 11, 5],
                           [1, 1, 5, 2],
                           [2, 1, 3, 2],
                           [1, 1, 3, 4]]
        self.column_two = [[2],
                           [1],
                           [-3],
                           [-3]]
        self.solution_two = [[-2],
                             [0],
                             [1],
                             [-1]]

        self.matrix_three = [[2, 5, 4, 1],
                             [1, 3, 2, 1],
                             [2, 10, 9, 7],
                             [3, 8, 9, 2]]
        self.column_three = [[20],
                             [11],
                             [40],
                             [37]]
        self.solution_three = [[1],
                               [2],
                               [2],
                               [0]]

    def test_gauss(self):
        """Test gauss evaluating."""
        solution = mat.solve(self.matrix_one, self.column_one)
        self.assertTrue(mat.compare_matrix(solution, self.solution_one))
        solution = mat.solve(self.matrix_two, self.column_two)
        self.assertTrue(mat.compare_matrix(solution, self.solution_two))
        solution = mat.solve(self.matrix_three, self.column_three)
        self.assertTrue(mat.compare_matrix(solution, self.solution_three))

    def test_gauss_without_leading(self):
        """Test gauss evaluating without leading coefficient optimization."""
        solution = mat.solve(self.matrix_one, self.column_one,
                             leading_coefficient=False)
        self.assertTrue(mat.compare_matrix(solution, self.solution_one))
        solution = mat.solve(self.matrix_two, self.column_two,
                             leading_coefficient=False)
        self.assertTrue(mat.compare_matrix(solution, self.solution_two))
        solution = mat.solve(self.matrix_three, self.column_three,
                             leading_coefficient=False)
        self.assertTrue(mat.compare_matrix(solution, self.solution_three))


if __name__ == '__main__':
    doctest.testmod(mat)
    doctest.testmod(gauss)
    unittest.main()
