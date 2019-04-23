#!/usr/bin/env python
import argparse

import numpy as np
from skimage.io import imread, imsave
from tqdm.autonotebook import tqdm
from skimage.filters import gaussian
from skimage.util import img_as_float

from utils import save_mask, display_snake

from scipy.interpolate import RectBivariateSpline
from skimage.filters import sobel

def active_contour(img,
                   init_snake,
                   alpha,
                   beta,
                   kappa,
                   tau,
                   w_line,
                   w_edge
                  ):
    edge = sobel(img)
    img = w_line*img + w_edge*edge

    intp = RectBivariateSpline(np.arange(img.shape[1]),
                               np.arange(img.shape[0]),
                               img.T, kx=2, ky=2, s=0)

    x = init_snake[:, 0]
    y = init_snake[:, 1]
    n = init_snake.shape[0]

    A = ((2 * alpha + 6 * beta) * np.eye(n) +
         (-alpha - 4 * beta) * np.roll(np.eye(n), -1, axis=0) +
         (-alpha - 4 * beta) * np.roll(np.eye(n), 1, axis=0) +
         beta * np.roll(np.eye(n), -2, axis=0) +
         beta * np.roll(np.eye(n), 2, axis=0)
        )

    invA = np.linalg.inv(tau * A + np.eye(n))

    for i in range(1000):
        fx = intp(x, y, dx=1, grid=False)
        fy = intp(x, y, dy=1, grid=False)
        xn = invA @ (x + tau*fx)
        yn = invA @ (y + tau*fy)

        x, y = xn, yn

    return np.array([x, y]).T

def process_image(args):
    img = img_as_float(imread(args.input_image))
    init_snake = np.loadtxt(args.initial_snake).astype(np.float32)
    img = gaussian(img, 3)

    result_snake = active_contour(
        img,
        init_snake,
        alpha=args.alpha,
        beta=args.beta,
        tau=args.tau,
        w_line=args.w_line,
        w_edge=args.w_edge,
        kappa=args.kappa
    )

    # display_snake(img, init_snake, result_snake)

    save_mask(args.output_image, result_snake, img)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('input_image')
    parser.add_argument('initial_snake')
    parser.add_argument('output_image')
    parser.add_argument('alpha', type=float)
    parser.add_argument('beta', type=float)
    parser.add_argument('tau', type=float)
    parser.add_argument('w_line', type=float)
    parser.add_argument('w_edge', type=float)
    parser.add_argument('kappa', type=float)

    args = parser.parse_args()

    process_image(args)

if __name__ == "__main__":
    main()
