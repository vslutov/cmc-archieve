import os.path
import argparse
import math

import numpy as np
from skimage import io
from itertools import product
from scipy.signal import fftconvolve, convolve2d

from .hysteresis import hysteresis

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

def mse(first_image, second_image):
    return ((first_image - second_image) ** 2).mean()

def psnr(first_image, second_image):
    return 20 * np.log10(255) - 10 * np.log10(mse(first_image, second_image))

def ssim(x, y):
    mu_x = x.mean()
    mu_y = y.mean()
    sigma_2_x = ((x - mu_x) ** 2).mean()
    sigma_2_y = ((y - mu_y) ** 2).mean()
    cov = ((x - mu_x) * (y - mu_y)).mean()
    L = 255
    c_1 = 0.01 * L
    c_2 = 0.03 * L
    return (2 * mu_x * mu_y + c_1) * (2 * cov + c_2) / ((mu_x ** 2 + mu_y ** 2 + c_1) * (sigma_2_x + sigma_2_y + c_2))

def mssim(x, y):
    BLOCK_SIZE = 8

    result = 0
    count = 0

    for i in range(0, x.shape[0], BLOCK_SIZE):
        for j in range(0, x.shape[1], BLOCK_SIZE):
            result += ssim(x[i:i + BLOCK_SIZE,
                             j:j + BLOCK_SIZE],
                           y[i:i + BLOCK_SIZE,
                             j:j + BLOCK_SIZE])

            count += 1

    return result / count

def canny(image, args):
    sigma = args.sigma
    size = int(np.ceil(sigma * 3))
    y = x = np.linspace(-size, size, 2 * size + 1)
    x, y = x[:, np.newaxis], y[np.newaxis, :]
    squares = x ** 2 + y ** 2
    gauss_kernel = np.exp(-squares / (2 * sigma ** 2)) / (2 * np.pi * sigma ** 2)
    gx = -gauss_kernel * x / sigma ** 2
    gy = -gauss_kernel * y / sigma ** 2

    image = grayscale(image)
    dx = convolve2d(image, gx, mode="same", boundary="symm")
    dy = convolve2d(image, gy, mode="same", boundary="symm")

    g_abs = dx ** 2 + dy ** 2
    g_ang = np.arctan2(dx, dy)

    ang_cat = ((g_ang + np.pi / 8) // (np.pi / 4)).astype(np.int32) % 4

    bordered_image = add_border(g_abs, 1, 1)

    xi, yi = np.where(ang_cat == 2)
    indeces = bordered_image[:-2, 1:-1][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0
    indeces = bordered_image[2:, 1:-1][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0

    xi, yi = np.where(ang_cat == 0)
    indeces = bordered_image[1:-1, :-2][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0
    indeces = bordered_image[1:-1, 2:][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0

    xi, yi = np.where(ang_cat == 1)
    indeces = bordered_image[:-2, :-2][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0
    indeces = bordered_image[2:, 2:][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0

    xi, yi = np.where(ang_cat == 3)
    indeces = bordered_image[:-2, 2:][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0
    indeces = bordered_image[2:, :-2][xi, yi] > g_abs[xi, yi]
    g_abs[xi[indeces], yi[indeces]] = 0

    result = np.zeros(g_abs.shape, dtype=np.uint8)
    result[g_abs > args.thr_high / 255.0] = 1
    mask = (g_abs > args.thr_low / 255.0).astype(np.uint8)

    result = hysteresis(result, mask)

    return result.astype(np.float32)

def gabor(image, args):
    size = int(np.ceil(args.sigma * 3))
    y = x = np.linspace(-size, size, 2 * size + 1)

    theta = args.theta / 180.0 * np.pi
    psi = args.psi / 180.0 * np.pi

    x, y = x[:, np.newaxis], y[np.newaxis, :]
    x, y = x * np.cos(theta) + y * np.sin(theta), -x * np.sin(theta) + y * np.cos(theta)

    gabor_filter = (np.exp(-(x ** 2 + args.gamma ** 2 * y ** 2) / (2 * args.sigma ** 2)) *
                    np.cos(2 * np.pi * x / getattr(args, "lambda") + psi)
                   )

    image = add_border(grayscale(image), size, size)
    image = fftconvolve(image, gabor_filter, mode="valid")
    # image[image < 0] = 0

    return add_background(image)

def vessels(image, args):
    size = int(np.ceil(args.sigma * 3))
    image = grayscale(image)
    result = np.zeros(image.shape)

    for lmbda, theta in product(np.linspace(2, 3, 3),
                                np.linspace(0, 90, 6),
                               ):

        lmbda *= args.sigma
        theta *= np.pi / 180.0

        y = x = np.linspace(-size, size, 2 * size + 1)
        x, y = x[:, np.newaxis], y[np.newaxis, :]
        x, y = x * np.cos(theta) + y * np.sin(theta), -x * np.sin(theta) + y * np.cos(theta)

        gabor_filter = (np.exp(-(x ** 2 + y ** 2) / (2 * args.sigma ** 2)) *
                        np.sin(2 * np.pi * x / lmbda)
                       )

        result = np.maximum(result, fftconvolve(image, gabor_filter, mode="same"))

    result -= result.min()
    result /= result.max()

    return result

def main():
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers()

    subparsers.add_parser("mse").set_defaults(metric=mse)
    subparsers.add_parser("psnr").set_defaults(metric=psnr)
    subparsers.add_parser("ssim").set_defaults(metric=ssim)
    subparsers.add_parser("mssim").set_defaults(metric=mssim)

    gabor_parser = subparsers.add_parser("gabor")
    gabor_parser.add_argument("sigma", type=float)
    gabor_parser.add_argument("gamma", type=float)
    gabor_parser.add_argument("theta", type=float)
    gabor_parser.add_argument("lambda", type=float)
    gabor_parser.add_argument("psi", type=float)
    gabor_parser.set_defaults(convert=gabor)

    canny_parser = subparsers.add_parser("canny")
    canny_parser.add_argument("sigma", type=float)
    canny_parser.add_argument("thr_high", type=float)
    canny_parser.add_argument("thr_low", type=float)
    canny_parser.set_defaults(convert=canny)

    vessels_parser = subparsers.add_parser("vessels")
    vessels_parser.add_argument("sigma", type=float)
    vessels_parser.set_defaults(convert=vessels)

    parser.add_argument("first_image")
    parser.add_argument("second_image")

    args = parser.parse_args()

    first_image = io.imread(args.first_image)

    if "metric" in dir(args):
        second_image = io.imread(args.second_image)

        if first_image.shape != second_image.shape:
            raise ValueError("Image shapes doesn't match: {shape1} and {shape2}."
                             .format(shape1=first_image.shape, shape2=second_image.shape))

        print(args.metric(first_image, second_image))

    elif "convert" in dir(args):
        first_image = first_image.astype(float) / 255
        second_image = args.convert(image=first_image, args=args)
        io.imsave(args.second_image, second_image)

    else:
        raise ValueError()
