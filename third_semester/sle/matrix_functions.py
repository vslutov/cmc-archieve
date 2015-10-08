"""
Implement usefull matrix procedures.

Operates with decimal values.
"""

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

from copy import deepcopy
from decimal import Decimal

EPS = Decimal('1e-10')


def matrix_to_string(matrix):
    """
    Return string representation of matrix.

    >>> print(matrix_to_string( [[100, 2, 3], [-1, 20, 3]] ))
    [[100  2 3]
     [ -1 20 3]]
    """
    matrix = decimal_matrix(matrix)

    height = len(matrix)
    if height == 0:
        return '[]'

    width = len(matrix[0])
    column_lens = [0] * width

    for row in matrix:
        for j in xrange(width):
            column_lens[j] = max(column_lens[j], len(str(row[j])))

    def row_to_string(row):
        """Convert matrix row into string."""
        item = lambda i: row[i].to_eng_string().rjust(column_lens[i])
        return '[' + ' '.join([item(i) for i in xrange(width)]) + ']'

    return '[' + '\n '.join([row_to_string(row) for row in matrix]) + ']'


def compare(matrix, another):
    """
    Return True if decimal matrix equals, False otherwise.

    >>> compare([[1, 2], [3, 4]], [[1, 2], [3, 4]])
    True
    >>> compare([[1, 1], [1, 1]], [[1, 2], [3, 4]])
    False
    """
    return standard_deviation(matrix, another) <= EPS


def decimal_matrix(matrix):
    """
    Return decimal matrix.

    >>> decimal_matrix([[1, 2], [3, 4]])
    [[Decimal('1'), Decimal('2')], [Decimal('3'), Decimal('4')]]
    """
    height = len(matrix)
    if height == 0:
        return []

    width = len(matrix[0])
    matrix = deepcopy(matrix)

    for row in matrix:
        for j in xrange(width):
            row[j] = Decimal(row[j])

    return matrix


def det(matrix):
    """
    Return determinant of the matrix.

    >>> a = det([[1, 0, 0],
    ...          [0, 1, 0],
    ...          [0, 0, 1]])
    >>> abs(a - 1) <= EPS
    True
    >>> a = det([[1, 2], [3, 4]])
    >>> abs(a - (-2)) <= EPS
    True
    """
    size = len(matrix)
    matrix = triangulate(matrix)

    result = 1
    for i in xrange(size):
        result *= matrix[i][i]

    return result


def ident(size):
    """
    Return identity matrix with size n.

    >>> print(matrix_to_string( ident(3) ))
    [[1 0 0]
     [0 1 0]
     [0 0 1]]
    """
    matrix = [range(size) for i in xrange(size)]

    for i in xrange(size):
        for j in xrange(size):
            matrix[i][j] = Decimal(1) if i == j else Decimal(0)

    return matrix


def inverse(matrix):
    """
    Return inverted matrix.

    >>> a = inverse([[1, 2], [3, 4]])
    >>> compare(a, [[-2, 1], [1.5, -0.5]])
    True

    If matrix determinant equals to zero, error will be raised.
    >>> inverse([[0, 0], [0, 0]])
    Traceback (most recent call last):
        ...
    ZeroDivisionError: matrix determinant equals to zero
    """
    size = len(matrix)
    identity = ident(size)
    return gauss_jordan(matrix, identity)


def solve(matrix, column, leading_coefficient=True):
    """
    Return solution of equation matrix * x = column.

    >>> a = solve([[1, 2], [3, 4]], [[1], [1]])
    >>> compare(a, [[-1], [1]])
    True
    """
    return gauss_jordan(matrix, column,
                        leading_coefficient=leading_coefficient)


def read_matrix_from_file(input_file):
    r"""
    Return matrix from file.

    >>> import os
    >>> f = os.tmpfile()
    >>> f.write('2 3\n1 2 3\n 4 5 6')
    >>> f.seek(0)
    >>> compare(read_matrix_from_file(f), [[1, 2, 3], [4, 5, 6]])
    True
    >>> f.close()
    """
    data = input_file.read().split()

    height = int(data[0])
    width = int(data[1])

    data = data[2:]
    data = [data[i * width:(i + 1) * width] for i in xrange(height)]
    return decimal_matrix(data)


def multiply(matrix_one, matrix_two):
    """
    Multiply pair of matrix.

    >>> a = multiply([[1, 2], [3, 4]], [[4, 3], [2, 1]])
    >>> print(matrix_to_string(a))
    [[ 8  5]
     [20 13]]
    """
    matrix_one = decimal_matrix(matrix_one)
    matrix_two = decimal_matrix(matrix_two)

    height = len(matrix_one)

    if height == 0:
        size = 0
    else:
        size = len(matrix_one[0])

    if size != len(matrix_two):
        raise ValueError('Unsuitable matrix sizes.')

    width = len(matrix_two[0])

    matrix = [range(width) for i in xrange(height)]

    for i in xrange(height):
        for j in xrange(width):
            matrix[i][j] = 0
            for k in xrange(size):
                matrix[i][j] += matrix_one[i][k] * matrix_two[k][j]

    return matrix


def standard_deviation(matrix_one, matrix_two):
    """
    Return standard deviation of pair of matrix.

    >>> a = standard_deviation([[1, 2], [3, 4]], [[1, 1], [1, 1]])
    >>> abs(a - Decimal('1.87082869339')) <= Decimal('1e-6')
    True
    """
    matrix_one = decimal_matrix(matrix_one)
    matrix_two = decimal_matrix(matrix_two)

    height = len(matrix_one)

    if height == 0:
        width = 0
    else:
        width = len(matrix_one[0])

    if (height != len(matrix_two) or
            (height != 0 and width != len(matrix_two[0]))):
        raise ValueError('Unsuitable matrix sizes.')

    result = 0

    for i in xrange(height):
        for j in xrange(width):
            result += (matrix_one[i][j] - matrix_two[i][j]) ** 2

    result = (result / (width * height))
    result **= Decimal('0.5')
    return result


def triangulate(matrix, leading_coefficient=True):
    """
    Return triangular form of the matrix.

    >>> a = triangulate([[1, 2], [3, 4]])
    >>> compare(a, [[-3, -4], [0, 2/3]])
    True

    Supports eliminate without leading coefficient optimisation.
    >>> a = triangulate([[1, 2], [3, 4]], leading_coefficient=False)
    >>> compare(a, [[1, 2], [0, -2]])
    True
    """
    matrix = decimal_matrix(matrix)

    height = len(matrix)
    if height == 0:
        return []
    width = len(matrix[0])

    for i in xrange(height):
        max_pivot = i
        for k in xrange(i, width):
            if (leading_coefficient and
                    abs(matrix[k][i]) > abs(matrix[max_pivot][i])):
                max_pivot = k
            elif not leading_coefficient and abs(matrix[k][i]) > EPS:
                max_pivot = k
                break

        if max_pivot != i:
            matrix[i], matrix[max_pivot] = matrix[max_pivot], matrix[i]
            for j in xrange(width):
                matrix[i][j] *= -1

        if abs(matrix[i][i]) > EPS:
            for k in xrange(i + 1, height):
                multiplier = matrix[k][i] / matrix[i][i]
                for j in xrange(i, width):
                    matrix[k][j] -= multiplier * matrix[i][j]

    return matrix


def gauss_jordan(matrix, another, leading_coefficient=True):
    """
    Make Gauss-Jordan elimination with matrixes.

    >>> A = [[1, 2], [3, 4]]
    >>> b = [[1], [1]]

    Solution of Ax = b.
    >>> x = gauss_jordan(A, b)
    >>> compare(x, [[-1], [1]])
    True

    Inverse matrix.
    >>> r = gauss_jordan(A, ident(len(A)))
    >>> compare(r, [[-2, 1], [1.5, -0.5]])
    True
    """
    matrix = decimal_matrix(matrix)
    if abs(det(matrix)) <= EPS:
        raise ZeroDivisionError('matrix determinant equals to zero')

    size = len(matrix)

    another = decimal_matrix(another)
    another_size = len(another[0])

    for i in xrange(size):

        max_pivot = i
        for k in xrange(i, size):
            if (leading_coefficient and
                    abs(matrix[k][i]) > abs(matrix[max_pivot][i])):
                max_pivot = k
            elif not leading_coefficient and abs(matrix[k][i]) > EPS:
                max_pivot = k
                break
        matrix[max_pivot], matrix[i] = matrix[i], matrix[max_pivot]
        another[max_pivot], another[i] = another[i], another[max_pivot]

        divider = matrix[i][i]
        for j in xrange(size):
            matrix[i][j] /= divider
        for j in xrange(another_size):
            another[i][j] /= divider

        for k in xrange(0, size):    # Eliminate column y
            if k != i:
                multiplier = matrix[k][i] / matrix[i][i]
                for j in xrange(size):
                    matrix[k][j] -= multiplier * matrix[i][j]
                for j in xrange(another_size):
                    another[k][j] -= multiplier * another[i][j]

    return another


def over_relax_solve(matrix, column, omega):
    """
    Find system of linear equations solution using the method of successive
    over-relaxation.

    Works good with positive-defined matrix and 0 < omega < 2.

    >>> A, b = [[1, 2], [2, 6]], [[1], [4]]
    >>> x = over_relax_solve(A, b, 0.5)
    >>> compare(multiply(A, x), b)
    True
    """
    size = len(matrix)
    sol = decimal_matrix([[0] for i in xrange(size)])
    omega = Decimal(omega)
    exactly = False
    while not exactly:
        prev = deepcopy(sol)
        for i in xrange(size):
            var = sum(matrix[i][j] * sol[j][0] for j in xrange(i))
            var += sum(matrix[i][j] * prev[j][0] for j in xrange(i, size))
            sol[i][0] += (column[i][0] - var) * omega / matrix[i][i]

        exactly = compare(multiply(matrix, sol), column)

    return sol


if __name__ == '__main__':
    import doctest
    doctest.testmod()
