"""Formula for calcutate element of the matrix."""

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

import matrix_functions as mat

SIZE = 10  # Change SIZE here


def augmented_matrix():
    """Return hardcoded augment matrix."""
    def formula(i, j):
        """Calculate a[i][j] square matrix element."""
        if i != j:  # Change matrix formula here
            return (i + j) / (2 * SIZE)
        else:
            return SIZE + SIZE ** 2 + i / SIZE

    matrix = [[formula(i, j) for j in xrange(SIZE)] for i in xrange(SIZE)]
    matrix = mat.multiply(matrix, matrix)

    def col_formula(i):
        """Calculate column element."""
        return i ** 2 - SIZE  # Change column formula here

    for i in xrange(SIZE):
        matrix[i].append(col_formula(i))

    return mat.decimal_matrix(matrix)
