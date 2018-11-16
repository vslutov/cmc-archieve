#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np


def prod_non_zero_diag(X):
    """Compute product of nonzero elements from matrix diagonal.

    input:
    x -- 2-d numpy array
    output:
    product -- integer number


    Vectorized implementation.
    """
    x = np.diag(X)
    return np.prod(x[x != 0])


def are_multisets_equal(x, y):
    """Return True if both vectors create equal multisets.

    input:
    x, y -- 1-d numpy arrays
    output:
    True if multisets are equal, False otherwise -- boolean

    Vectorized implementation.
    """
    return np.array_equal(np.sort(x), np.sort(y))


def max_after_zero(x):
    """Find max element after zero in array.

    input:
    x -- 1-d numpy array
    output:
    maximum element after zero -- integer number

    Vectorized implementation.
    """
    indeces = np.roll(x == 0, 1)
    indeces[0] = False
    return np.max(x[indeces])


def convert_image(image, weights):
    """Sum up image channels with weights from coefs array

    input:
    img -- 3-d numpy array (H x W x 3)
    coefs -- 1-d numpy array (length 3)
    output:
    img -- 2-d numpy array

    Vectorized implementation.
    """
    return np.dot(weights, image.transpose((0, 2, 1)))


def run_length_encoding(x):
    """Make run-length encoding.

    input:
    x -- 1-d numpy array
    output:
    elements, counters -- integer iterables

    Vectorized implementation.
    """
    # Copied from: https://gist.github.com/nvictus/66627b580c13068589957d6ab0919e66
    n = len(x)
    starts = np.r_[0, np.flatnonzero(~np.isclose(x[1:], x[:-1], equal_nan=True)) + 1]
    return x[starts], np.diff(np.r_[starts, n])


def pairwise_distance(X, Y):
    """Return pairwise object distance.

    input:
    x, y -- 2d numpy arrays
    output:
    distance array -- 2d numpy array

    Vctorized implementation.
    """
    X = np.array([X] * len(Y)).transpose((1, 0, 2))
    Y = np.array([Y] * len(X))
    return np.sqrt(((X - Y) ** 2).sum(axis=2))
