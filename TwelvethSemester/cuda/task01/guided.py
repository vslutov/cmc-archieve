from skimage.io import imread, imsave
from skimage import img_as_float32, img_as_ubyte
from scipy.fftpack import fft, ifft
import numpy as np
from scipy.signal import convolve2d

image = imread('astronaut.png')
image = img_as_float32(image)
image = image.clip(min=1e-6)

r = 5
eps = 0.15 ** 2
conv_kernel = np.ones((r, r))
conv_kernel /= conv_kernel.sum()

for i in range(image.shape[2]):
    layer = image[:, :, i]

    mean_layer = convolve2d(layer, conv_kernel, mode='same', boundary='symm')
    corr_layer = convolve2d(layer ** 2, conv_kernel, mode='same', boundary='symm')
    var_layer = corr_layer - mean_layer ** 2

    a = var_layer / (var_layer + eps)
    b = (1 - a) * mean_layer

    mean_a = convolve2d(a, conv_kernel, mode='same', boundary='symm')
    mean_b = convolve2d(b, conv_kernel, mode='same', boundary='symm')

    layer = mean_a * layer + mean_b

    image[:, :, i] = layer

image = image.clip(0, 1)
imsave('python.png', img_as_ubyte(image))
