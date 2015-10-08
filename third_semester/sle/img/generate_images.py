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

import numpy as np
import matplotlib.pyplot as plt

f = open('gauss-leading.txt', 'r')
leading = [ line.split() for line in f ]
leading = np.array([[float(elem) for elem in row] for row in leading])
leading = leading.transpose()
leading_size, leading_residual, leading_time = leading
f.close()

f = open('gauss-without.txt', 'r')
without = [line.split() for line in f]
without = np.array([[float(elem) for elem in row] for row in without])
without = without.transpose()
without_size, without_residual, without_time = without
f.close()

f = open('over-relax.txt', 'r')
relax = [line.split() for line in f]
relax = [[float(elem) for elem in row] for row in relax]
relax = [np.array(relax[i::8]).transpose()[::2] for i in xrange(8)]
f.close()

plt.figure(figsize=(7, 4))
plt.axes((0.15, 0.1, 0.35, 0.8))
plt.grid(True)
plt.box(on=False)
plt.title("Residual")
plt.semilogy(leading_size, leading_residual, "c.", label='with leading coefficient')
plt.semilogy(without_size, without_residual, "k-", label='without leading coefficient')
plt.legend()

plt.axes((0.6, 0.1, 0.35, 0.8))
plt.grid(True)
plt.box(on=False)
plt.title("Time (sec)")
plt.plot(leading_size, leading_time, "c.", without_size, without_time, "k-")

plt.savefig('gauss_stat.png')


plt.figure(figsize=(8, 4))
plt.axes((0.1, 0.1, 0.8, 0.8))
plt.grid(True)
plt.box(on=False)
plt.title("Time (sec)")
plt.plot(relax[1][0], relax[1][1], 'r*', label='omega = 0.4 and omega = 1.6')
plt.plot(relax[2][0], relax[2][1], 'g--', label='omega = 0.6 and omega = 1.4')
plt.plot(relax[3][0], relax[3][1], 'b+', label='omega = 0.8 and omega = 1.2')
plt.plot(relax[4][0], relax[4][1], 'k-', label='omega = 1.0')
plt.plot(leading_size, leading_time, 'mx', label='gauss')
plt.legend(loc='upper left')

plt.savefig('over_relax_stat.png')

