#!/usr/bin/env python3
# -*- coding utf-8 -*-

import argparse

from skimage.io import imsave
from skimage import img_as_ubyte
import numpy as np

def generate_kernel(gauss_sigma):
    half_size = np.ceil(3 * gauss_sigma).astype(np.int32)
    full_size = 2 * half_size + 1
    x = np.linspace(-half_size, half_size, full_size).reshape((full_size, 1))
    y = x.reshape((1, full_size))
    kernel = np.exp(-(x ** 2 + y ** 2) / (2 * gauss_sigma ** 2))
    kernel /= kernel.max()
    return kernel

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('output_image')
    parser.add_argument('gauss_sigma', type=float)

    args = parser.parse_args()

    kernel = generate_kernel(args.gauss_sigma)
    imsave(args.output_image, img_as_ubyte(kernel))

if __name__ == "__main__":
    main()

