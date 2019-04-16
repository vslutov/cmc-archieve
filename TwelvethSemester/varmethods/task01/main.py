#!/usr/bin/env python3
# -*- coding utf-8 -*-

import argparse

from skimage.io import imread, imsave
from skimage import img_as_float32, img_as_ubyte
from skimage.color import gray2rgb, rgb2gray, rgb2ycbcr, ycbcr2rgb
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

Q = ((0, 1), (1, 0), (1, 1))

def btv(image):
    value = 0
    grad = ZeroIsALie(np.zeros_like(image))
    image = ZeroIsALie(image)

    for i, j in Q:
        diff = image[i:, j:] - image[:-i, :-j]
        value += np.abs(diff).sum() / (len(Q) * np.sqrt(i ** 2 + j ** 2))
        grad[i:, j:] += np.sign(diff)
        grad[:-i, :-j] -= np.sign(diff)

    return value, grad.array

def btv2(image):
    value = 0
    grad = ZeroIsALie(np.zeros_like(image))
    image = ZeroIsALie(image)

    for i, j in Q:
        diff = image[2 * i:, 2 * j:] + image[:-2 * i, :-2 * j] - 2 * image[i:-i, j:-j]
        value += np.abs(diff).sum() / (len(Q) * np.sqrt(i ** 2 + j ** 2))
        grad[2 * i:, 2 * j:] += np.sign(diff)
        grad[:-2 * i, :-2 * j] += np.sign(diff)
        grad[i:-i, j:-j] -= 2 * np.sign(diff)

    return value, grad.array

def tgv(image):
    btv_value, btv_grad = btv(image)
    btv2_value, btv2_grad = btv2(image)
    btv_coef, btv2_coef = 0.01, 0.01

    return btv_value * btv_coef + btv2_value * btv2_coef, btv_grad * btv_coef + btv2_grad * btv2_coef

def dist(target, image, kernel):
    N, M, C = target.shape
    K, L = kernel.shape

    diff = convolve(target, kernel) - image
    dist_value = np.abs(diff).sum()

    dist_grad = np.zeros_like(target)
    for k in range(K):
        for l in range(L):
            dist_grad[k:k + N - K + 1, l:l + M - L + 1] += kernel[k, l] * diff

    return dist_value, dist_grad

def deconvolve(image, kernel, noise_level):
    N, M, C = image.shape
    K, L = kernel.shape
    N += K - 1
    M += L - 1

    result = target = rgb2ycbcr(np.zeros((N, M, C)).astype(image.dtype) + 0.5)
    # target[K // 2:K // 2 + image.shape[0], L // 2:L // 2 + image.shape[1], 0] = image[:, :, 0]
    momentum = np.zeros_like(result)
    best_loss = float('+inf')

    for i in tqdm(range(40), desc='iteration'):
        lr = 0.25 * 0.01 ** ((i + 1) / 40.0)

        nesterov_target = target + momentum

        dist_value, dist_grad = dist(nesterov_target, image, kernel)
        tgv_value, tgv_grad = tgv(nesterov_target)

        loss = dist_value + tgv_value
        grad_target = dist_grad + tgv_grad

        momentum = 0.85 * momentum - lr * grad_target
        target += momentum

        if loss < best_loss:
            result = target

        imsave('tmp.png', img_as_ubyte(ycbcr2rgb(result).clip(0, 1)))

    return result


def denoise(args):
    kernel = img_as_float32(imread(args.kernel))
    if len(kernel.shape) == 3:
        kernel = rgb2gray(kernel)
    kernel /= kernel.sum()

    image = img_as_float32(imread(args.input_image))
    if len(image.shape) == 2:
        image = gray2rgb(image)
    image = rgb2ycbcr(image)

    result = deconvolve(image, kernel, args.noise_level)

    imsave(args.output_image, img_as_ubyte(ycbcr2rgb(result).clip(0, 1)))

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('input_image')
    parser.add_argument('kernel')
    parser.add_argument('output_image')
    parser.add_argument('noise_level', type=int)

    args = parser.parse_args()

    denoise(args)

if __name__ == "__main__":
    np.random.seed(0)
    main()
