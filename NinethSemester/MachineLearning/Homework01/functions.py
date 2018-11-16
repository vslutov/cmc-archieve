#!/usr/bin/env python
# -*- coding: utf-8 -*-

from math import sqrt


def prod_non_zero_diag(X):
    """Compute product of nonzero elements from matrix diagonal.

    input:
    x -- 2-d numpy array
    output:
    product -- integer number


    Not vectorized implementation.
    """
    if len(X) == 0:
        return 1
    result = 1
    for i in range(min(len(X), len(X[0]))):
        if X[i][i] != 0:
            result *= X[i][i]
    return result


def are_multisets_equal(x, y):
    """Return True if both vectors create equal multisets.

    input:
    x, y -- 1-d numpy arrays
    output:
    True if multisets are equal, False otherwise -- boolean

    Not vectorized implementation.
    """
    return sorted(x) == sorted(y)


def max_after_zero(x):
    """Find max element after zero in array.

    input:
    x -- 1-d numpy array
    output:
    maximum element after zero -- integer number

    Not vectorized implementation.
    """
    m = None
    before_zero = False
    for c in x:
        if before_zero and (m is None or m < c):
            m = c
        if c == 0:
            before_zero = True
        else:
            before_zero = False
    return m


def dot(x, y):
    return sum(c1 * c2 for c1, c2 in zip(x, y))


def convert_image(image, weights):
    """Sum up image channels with weights from coefs array

    input:
    img -- 3-d numpy array (H x W x 3)
    coefs -- 1-d numpy array (length 3)
    output:
    img -- 2-d numpy array

    Not vectorized implementation.
    """
    return [[dot(pixel, weights) for pixel in line] for line in image]


def run_length_encoding(x):
    """Make run-length encoding.

    input:
    x -- 1-d numpy array
    output:
    elements, counters -- integer iterables

    Not vectorized implementation.
    """
    codes, counts = [], []
    e, c = x[0], 1
    for n in x[1:]:
        if e == n:
            c += 1
        else:
            codes.append(e)
            counts.append(c)
            e, c = n, 1
    codes.append(e)
    counts.append(c)

    return codes, counts


def dist(x, y):
    return sqrt(sum((c1 - c2) ** 2 for c1, c2 in zip(x, y)))


def pairwise_distance(X, Y):
    """Return pairwise object distance.

    input:
    x, y -- 2d numpy arrays
    output:
    distance array -- 2d numpy array

    Not vectorized implementation.
    """
    return [[dist(x, y) for y in Y] for x in X]
