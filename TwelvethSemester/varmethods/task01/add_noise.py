#!/usr/bin/env python3
# -*- coding utf-8 -*-

import argparse

from skimage.io import imread, imsave
from skimage import img_as_float32, img_as_ubyte
from skimage.color import gray2rgb, rgb2gray
import numpy as np
from scipy.signal import convolve2d

def add_noise(args):
    kernel = img_as_float32(imread(args.kernel))
    if len(kernel.shape) == 3:
        kernel = rgb2gray(kernel)
    kernel /= kernel.sum()

    image = img_as_float32(imread(args.input_image))
    if len(image.shape) == 2:
        image = gray2rgb(image)

    noised = np.dstack([convolve2d(image[:, :, channel], kernel, 'valid') for channel in range(image.shape[2])])
    noised += np.random.normal(0, args.noise_level / 255.0, noised.shape)
    noised = noised.clip(0, 1)

    imsave(args.output_image, img_as_ubyte(noised))

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('input_image')
    parser.add_argument('kernel')
    parser.add_argument('output_image')
    parser.add_argument('noise_level', type=int)

    args = parser.parse_args()

    add_noise(args)

if __name__ == "__main__":
    np.random.seed(0)
    main()
