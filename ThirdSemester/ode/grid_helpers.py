"""Ode helpfull grid functional methods."""

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


def sub(first, second):
    """
    Substract grid functions.

    >>> sub([[2, 3, 4]], [[1, 2, 3]])
    [[1, 1, 1]]
    """
    width = len(first)
    height = len(first[0])
    return [[first[i][j] - second[i][j] for j in xrange(height)]
            for i in xrange(width)]


def norm(func):
    """
    Calculate norm of grid function.

    >>> norm([[1, 2, 3]])
    3
    """
    return max(abs(y) for step in func for y in step)


if __name__ == '__main__':
    import doctest
    doctest.testmod()
