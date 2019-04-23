#!/usr/bin/env python3
# -*- coding utf-8 -*-

import argparse

from skimage.io import imread, imsave
from skimage import img_as_float32, img_as_ubyte
from skimage.color import gray2rgb, rgb2gray
import numpy as np
from scipy.signal import convolve2d
from tqdm.autonotebook import tqdm

def convolve(image, kernel):
    N, M, C = image.shape
    K, L = kernel.shape

    result = np.zeros((N - K + 1, M - L + 1, C), np.float32)

    for k in range(K):
        for l in range(L):
            result += kernel[k, l] * image[k:k + N - K + 1, l:l + M - L + 1]

    return result

def zero_is_a_lie(indeces):
    return tuple(
        slice(
            index.start if index.start != 0 else None,
            index.stop if index.stop != 0 else None,
            index.step
        ) for index in indeces
    )

class ZeroIsALie:
    def __init__(self, array):
        self.array = array

    def __getitem__(self, indeces):
        return self.array.__getitem__(zero_is_a_lie(indeces))

    def __setitem__(self, indeces, value):
        return self.array.__setitem__(zero_is_a_lie(indeces), value)

Q = np.array(((0, 1), (1, 0), (1, 1)))
Q_coef = 1 / np.sqrt((Q ** 2).sum(axis=1))

def btv(image):
    grad = ZeroIsALie(np.zeros_like(image))
    image = ZeroIsALie(image)

    for (i, j), c in zip(Q, Q_coef):
        diff = image[i:, j:] - image[:-i, :-j] * c
        grad[i:, j:] += np.sign(diff)
        grad[:-i, :-j] -= np.sign(diff)

    return grad.array

def btv2(image):
    grad = ZeroIsALie(np.zeros_like(image))
    image = ZeroIsALie(image)

    for (i, j), c in zip(Q, Q_coef):
        diff = image[2 * i:, 2 * j:] + image[:-2 * i, :-2 * j] - 2 * image[i:-i, j:-j] * c
        grad[2 * i:, 2 * j:] += np.sign(diff)
        grad[:-2 * i, :-2 * j] += np.sign(diff)
        grad[i:-i, j:-j] -= 2 * np.sign(diff)

    return grad.array

def tgv(image, noise_level):
    btv_grad = btv(image)
    btv2_grad = btv2(image)
    btv2_coef = btv_coef = 1e-4 + 19e-4 * noise_level / 20

    return btv_grad * btv_coef + btv2_grad * btv2_coef

def dist(target, image, kernel):
    N, M, C = target.shape
    K, L = kernel.shape

    diff = convolve(target, kernel) - image

    dist_grad = np.zeros_like(target)
    for k in range(K):
        for l in range(L):
            dist_grad[k:k + N - K + 1, l:l + M - L + 1] += kernel[k, l] * diff

    return dist_grad

def deconvolve(image, kernel, noise_level):
    N, M, C = image.shape
    K, L = kernel.shape
    N += K - 1
    M += L - 1

    target = (np.zeros((N, M, C)).astype(image.dtype) + 0.5)
    momentum = np.zeros_like(target)
    best_loss = float('+inf')

    for i in tqdm(range(100), desc='SGD'):
        lr = 0.25 * 0.01 ** (i / 100.0)

        nesterov_target = target + 0.8 * momentum

        dist_grad = dist(nesterov_target, image, kernel)
        tgv_grad = tgv(nesterov_target, noise_level)

        grad_target = dist_grad + tgv_grad

        momentum = - lr * grad_target + 0.8 * momentum
        target += momentum

    target = target[K // 2:K // 2 + image.shape[0], L // 2:L // 2 + image.shape[1]]

    return target


def denoise(args):
    kernel = img_as_float32(imread(args.kernel))
    if kernel.shape[2] == 4:
        kernel = kernel[:, :, :3]
    if len(kernel.shape) == 3:
        kernel = rgb2gray(kernel)
    kernel /= kernel.sum()

    image = img_as_float32(imread(args.input_image))
    if image.shape[2] == 4:
        image = image[:, :, :3]
    if len(image.shape) == 2:
        image = gray2rgb(image)
    image = (image)

    result = deconvolve(image, kernel, args.noise_level)

    imsave(args.output_image, img_as_ubyte((result).clip(0, 1)))

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('input_image')
    parser.add_argument('kernel')
    parser.add_argument('output_image')
    parser.add_argument('noise_level', type=float)

    args = parser.parse_args()

    denoise(args)

if __name__ == "__main__":
    np.random.seed(0)
    main()
