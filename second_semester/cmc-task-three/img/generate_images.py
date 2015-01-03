# -*- coding: utf-8 -*-
"""
 *  cmc-task-three - comparation of some sort method
 *  Copyright (C) 2014 V. S. Lutov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import numpy as np
import matplotlib.pyplot as plt

# Scan data

f = open ( 'data.txt' , 'r')
data = [ line.strip('\n ').split(' ') for line in f ]
f.close()

data = map(lambda a: map(float, a), filter(lambda a: a != [''], data))
data = np.array(data)
rows, cols = data.shape
data = np.array([a.transpose() for a in data.reshape(rows, cols / 5, 5)])
t = data[0][0]

# Canvas one

one = plt.figure(figsize=(9, 4)).number

# Comparations

plt.axes((0.1, 0.1, 0.25, 0.8))
plt.grid(True)
plt.box(on=False)
plt.title("Comparations")

f = t ** 2 / 2
g = f * 11 / 12
h = 2 * t

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "the worst", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "the best", fontsize=14, color="green")

# Swaps

plt.axes((0.55, 0.1, 0.25, 0.8))
plt.grid(True)
plt.box(on=False)
plt.ylim(-100, 5000)
plt.title("Swaps")

f = t** 2 / 2
g = f / 2
h = t * 0

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "the worst", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "the best", fontsize=14, color="green")


# Canvas two

two = plt.figure(figsize=(9, 4)).number

swaps = 0.8 * np.log2(t) * t
cmps = 2 * swaps

# Comparations

plt.subplot(1, 2, 1)
plt.grid(True)
plt.box(on=False)
plt.title("Comparations")

plt.plot(t, cmps, "b-")

# Swaps

plt.subplot(1, 2, 2)
plt.grid(True)
plt.box(on=False)
plt.title("Swaps")

plt.plot(t, swaps, "b-")


# Canvas three

three = plt.figure(figsize=(9, 9)).number

# Comparation

plt.axes((0.1, 0.55, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Comparations")

f = data[1][1]
g = data[2][1]
h = data[0][1]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "the worst", fontsize=14, color="red")
plt.text(100, g[-1] - 200, "average", fontsize=14, color="blue")
plt.text(100, h[-1], "the best", fontsize=14, color="green")

# Swaps

plt.axes((0.55, 0.55, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Swaps")
plt.ylim(-100, 5000)

f = data[1][2]
g = data[2][2]
h = data[0][2]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "the worst", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "the best", fontsize=14, color="green")

# Tacts

plt.axes((0.1, 0.1, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Tacts")

f = data[1][3]
g = data[2][3]
h = data[0][3]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "the worst", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "the best", fontsize=14, color="green")


# Canvas four

four = plt.figure(figsize=(9, 9)).number

# Comparation

plt.axes((0.1, 0.55, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Comparations")

f = data[4][1]
g = data[5][1]
h = data[3][1]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "reverse", fontsize=14, color="red")
plt.text(100, g[-1] - 20, "average", fontsize=14, color="blue")
plt.text(100, h[-1], "sorted", fontsize=14, color="green")

# Swaps

plt.axes((0.55, 0.55, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Swaps")

f = data[4][2]
g = data[5][2]
h = data[3][2]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "reverse", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "sorted", fontsize=14, color="green")

# Tacts

plt.axes((0.1, 0.1, 0.3, 0.4))
plt.grid(True)
plt.box(on=False)
plt.title("Tacts")

f = data[4][3]
g = data[5][3]
h = data[3][3]

plt.plot(t, f, "r-", t, g, "b-", t, h, "g-")

plt.text(100, f[-1], "reverse", fontsize=14, color="red")
plt.text(100, g[-1], "average", fontsize=14, color="blue")
plt.text(100, h[-1], "sorted", fontsize=14, color="green")


# Canvas five

five = plt.figure().number
plt.axes((0.1, 0.1, 0.7, 0.8))

plt.grid(True)
plt.box(on=False)
plt.title("Tacts")

f = data[1][3]
h = data[3][3]

plt.plot(t, f, "r-", t, h, "g-")

plt.text(100, f[-1], "bubble sort", fontsize=14, color="red")
plt.text(100, h[-1], "heap sort", fontsize=14, color="green")

"""
plt.figure(one)
plt.savefig("bubble_theory.png")

plt.figure(two)
plt.savefig("heap_theory.png")

plt.figure(three)
plt.savefig("bubble_real.png")

plt.figure(four)
plt.savefig("heap_real.png")

plt.figure(five)
plt.savefig("bubble_vs_heap.png")
"""
