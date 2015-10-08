"""Formulate the problem."""

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

from math import e, sin

FIRST = (lambda x, y: (3 - y - x,), (0, 0))
FIRST_SOLUTION = lambda x: (4 - x - 4 * e ** -x,)
SECOND = (lambda x, u, v: (sin(2 * u**2) + x + v, x + u - 2 * v**2 + 1),
          (0, 1, 0.5))


THIRD = (lambda x: (-2 * x, -2, -4 * x), ((1, -1, 0), (1, 0, 1 + e)))
THIRD_RANGE = (0, 1)
THIRD_SOLUTION = lambda x: x + e ** x ** 2

FORTH = (lambda x: (-0.5 * x, 1, 2), ((1, 0, 1.2), (1, 2, 1.4)))
FORTH_RANGE = (0.4, 0.3)
FORTH_SOLUTION = lambda x: x + e ** x ** 2
