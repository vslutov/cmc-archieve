import os.path
import argparse
import math

import numpy as np
from skimage import io
from itertools import product
from scipy.signal import fftconvolve, convolve2d

def grayscale(image):
    return np.dot(image, [0.299, 0.587, 0.114])

def add_border(image, border_rows, border_cols):
    if len(image.shape) > 2:
        return np.dstack([add_border(image=image[:, :, i], border_rows=border_rows, border_cols=border_cols) for i in range(image.shape[2])])

    a = image[border_rows-1::-1, border_cols-1::-1]
    b = image[border_rows-1::-1, :]
    c = image[border_rows-1::-1, :-border_cols-1:-1]

    d = image[:, border_cols-1::-1]
    e = image
    f = image[:, :-border_cols-1:-1]

    g = image[:-border_rows-1:-1, border_cols-1::-1]
    h = image[:-border_rows-1:-1, :]
    i = image[:-border_rows-1:-1, :-border_cols-1:-1]

    return np.block([[a, b, c],
                     [d, e, f],
                     [g, h, i]])

def add_background(foreground):
    if len(foreground.shape) > 2:
        return np.dstack([add_background(foreground[:, :, i]) for i in range(foreground.shape[2])])

    foreground = foreground.copy()
    foreground[foreground > 1] = 1
    foreground[foreground < -1] = -1
    # foreground -= foreground.min()
    foreground += 1
    foreground /= 2
    return foreground

def bilateral(image, args):
    if len(image.shape) == 3:
        return np.dstack([bilateral(image[:, :, i], args) for i in range(image.shape[2])])

    sigma_r = args.sigma_r / 255.0
    sigma_d = args.sigma_d

    size = int(np.ceil(sigma_d * 3))
    rows, cols = image.shape

    x = y = np.linspace(-size, size, 2 * size + 1)
    coord_dist = (x[:, np.newaxis] ** 2 + y[np.newaxis, :] ** 2) / sigma_d ** 2

    result = np.zeros(image.shape)
    image = add_border(image, size, size)

    for i in range(rows):
        for j in range(cols):
            values = image[i:i + 2 * size + 1, j:j + 2 * size + 1]
            val_dist = (values - values[size, size]) ** 2 / sigma_r ** 2
            weights = np.exp(-(val_dist + coord_dist) * 0.5)
            weights /= weights.sum()
            result[i, j] = (values * weights).sum()

    return result

def harris(image, args):
    if len(image.shape) == 3:
        image = grayscale(image)

    sigma = args.sigma
    k = 0.04

    size = int(np.ceil(sigma * 3))
    rows, cols = image.shape

    x = y = np.linspace(-size, size, 2 * size + 1)
    coord_dist = (x[:, np.newaxis] ** 2 + y[np.newaxis, :] ** 2) / sigma ** 2
    w = np.exp(-coord_dist * 0.5)
    w /= w.sum()

    result = np.zeros(image.shape)
    r = np.zeros(image.shape)
    image = add_border(image, size + 1, size + 1) * 255.0

    dx = (image[2:, 1:-1] - image[:-2, 1:-1]) * 0.5
    dy = (image[1:-1, 2:] - image[1:-1, :-2]) * 0.5

    for i in range(rows):
        for j in range(cols):
            ddx = dx[i:i + 2 * size + 1, j:j + 2 * size + 1]
            ddy = dy[i:i + 2 * size + 1, j:j + 2 * size + 1]

            m = np.array([[(ddx ** 2 * w).sum(), (ddx * ddy * w).sum()],
                          [(ddx * ddy * w).sum(), (ddy ** 2 * w).sum()]])

            r[i, j] = np.linalg.det(m) - k * np.trace(m)

    for i in range(1, rows - 1):
        for j in range(1, cols - 1):
            if np.argmax(r[i - 1:i + 2, j - 1:j + 2]) == 4 and r[i, j] > 100000:
                result[i, j] = 1

    # result[result < result.mean() + 3 * result.std()] = 0
    # result[result > 0] = 1

    return result

def main():
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers()

    bilateral_parser = subparsers.add_parser("bilateral")
    bilateral_parser.add_argument("sigma_d", type=float)
    bilateral_parser.add_argument("sigma_r", type=float)
    bilateral_parser.set_defaults(convert=bilateral)

    harris_parser = subparsers.add_parser("harris")
    harris_parser.add_argument("sigma", type=float)
    harris_parser.set_defaults(convert=harris)

    parser.add_argument("input_image")
    parser.add_argument("output_image")

    args = parser.parse_args()

    input_image = io.imread(args.input_image)

    if "convert" in dir(args):
        input_image = input_image.astype(float) / 255
        output_image = args.convert(image=input_image, args=args)
        io.imsave(args.output_image, output_image)

    else:
        raise ValueError()
