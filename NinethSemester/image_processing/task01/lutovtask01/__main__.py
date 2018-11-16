import os.path
import argparse
import math

import numpy as np
from skimage import io

def mirror(image, args):
    if args.axis == "x":
        return image[:, ::-1]
    else:
        return image[::-1, :]

def rotate(image, args):
    if args.turn == "cw":
        angle = +args.angle % 360
    else:
        angle = -args.angle % 360

    if angle == 0:
        return image
    elif angle == 180:
        return image[::-1, ::-1]
    elif angle == 90:
        return image.transpose((1, 0, 2))[:, ::-1]
    elif angle == 270:
        return image.transpose((1, 0, 2))[::-1, :]

    c, s = np.cos(np.radians(angle)), np.sin(np.radians(angle))
    R = np.array([[c, -s], [s, c]])

    points = np.array([[0, 0],
                       [image.shape[0], 0],
                       [0, image.shape[1]],
                       [image.shape[0], image.shape[1]]])
    points = np.dot(points, R)

    min_corner = np.floor(np.min(points, axis=0)).astype(int)
    max_corner = np.ceil(np.max(points, axis=0)).astype(int)
    x = np.arange(min_corner[0], max_corner[0])
    y = np.arange(min_corner[1], max_corner[1])
    new = np.zeros((len(x), len(y), 2))
    new[:, :, 0] = x[:, np.newaxis]
    new[:, :, 1] = y[np.newaxis, :]

    new = np.dot(new, R.T)

    mask = np.ones((len(x), len(y), 3))
    mask[new[:, :, 0] < 0, :] = 0
    mask[new[:, :, 1] < 0, :] = 0
    mask[new[:, :, 0] >= image.shape[0], :] = 0
    mask[new[:, :, 1] >= image.shape[1], :] = 0

    def nearest(any_image):
        any_image = any_image.copy()
        any_image[any_image < 0] = 0
        any_image[any_image[:, :, 0] >= image.shape[0], 0] = image.shape[0] - 1
        any_image[any_image[:, :, 1] >= image.shape[1], 1] = image.shape[1] - 1
        return any_image

    ll = np.floor(new).astype(int)
    rr = np.ceil(new).astype(int)
    ll = nearest(ll)
    rr = nearest(rr)
    lr = np.dstack([ll[:, :, 0], rr[:, :, 1]])
    rl = np.dstack([rr[:, :, 0], ll[:, :, 1]])

    ll = ll.transpose((2, 0, 1))
    rr = rr.transpose((2, 0, 1))
    lr = lr.transpose((2, 0, 1))
    rl = rl.transpose((2, 0, 1))
    new = new.transpose((2, 0, 1))

    val_ll = image.__getitem__(list(ll))
    val_rr = image.__getitem__(list(rr))
    val_lr = image.__getitem__(list(lr))
    val_rl = image.__getitem__(list(rl))

    val_l = (new[1] - ll[1])[:, :, np.newaxis] * val_lr + (lr[1] - new[1])[:, :, np.newaxis] * val_ll
    val_r = (new[1] - rl[1])[:, :, np.newaxis] * val_rr + (rr[1] - new[1])[:, :, np.newaxis] * val_rl
    val   = (new[0] - ll[0])[:, :, np.newaxis] * val_r  + (rr[0] - new[0])[:, :, np.newaxis] * val_l

    return val * mask

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

def apply_filter(image, filt):
    if len(image.shape) > 2:
        return np.dstack([apply_filter(image[:, :, i], filt) for i in range(image.shape[2])])

    filt = np.array(filt)
    input_shape = image.shape
    bordered_image = add_border(image=image, border_rows=filt.shape[0] // 2, border_cols=filt.shape[1] // 2)
    result = np.zeros(input_shape)

    for row in range(filt.shape[0]):
        for col in range(filt.shape[1]):
            result += bordered_image[row:row + input_shape[0], col:col + input_shape[1]] * filt[row][col]

    return result

def add_background(foreground):
    if len(foreground.shape) > 2:
        return np.dstack([add_background(foreground[:, :, i]) for i in range(foreground.shape[2])])

    foreground = foreground.copy()
    foreground[foreground > 1] = 1
    foreground[foreground < -1] = -1
    foreground -= foreground.min()
    foreground /= foreground.max()
    return foreground

def sobel(image, args):
    if args.axis == "x":
        result = apply_filter(image=image, filt=[[-1, 0, 1],
                                                 [-2, 0, 2],
                                                 [-1, 0, 1]])
    else:
        result = apply_filter(image=image, filt=[[-1, -2, -1],
                                                 [ 0,  0,  0],
                                                 [ 1,  2,  1]])
    return add_background(result)

def get_x_y_for_gauss(sigma):
    radius = math.ceil(3 * sigma)
    axis = np.linspace(-3 * sigma, 3 * sigma, 2 * radius + 1)
    x = axis[:, np.newaxis]
    y = axis[np.newaxis, :]
    return x, y

def gauss_kernel(sigma):
    x, y = get_x_y_for_gauss(sigma)
    squares = x ** 2 + y ** 2
    return np.exp(-squares / (2 * sigma ** 2)) / (2 * math.pi * sigma ** 2)

def gauss(image, args):
    image = image.copy()
    filt = gauss_kernel(args.sigma)

    image **= args.gamma
    image = apply_filter(image=image, filt=filt)
    image **= 1/args.gamma
    return image

def median(image, args):
    image_shape = image.shape
    diam = 2 * args.rad + 1
    bordered_image = add_border(image=image, border_rows=args.rad, border_cols=args.rad)
    slices = np.array([bordered_image[i:i + image_shape[0], j:j + image_shape[1]] for i in range(diam) for j in range(diam)])
    return np.median(slices, axis=0)

def gradient(image, args):
    sigma = args.sigma
    gray_image = grayscale(image)
    x, y = get_x_y_for_gauss(sigma)
    gx = -gauss_kernel(sigma) * x / sigma ** 2
    gy = -gauss_kernel(sigma) * y / sigma ** 2
    result = np.sqrt(apply_filter(gray_image, gx) ** 2 + apply_filter(gray_image, gy) ** 2)
    print(result.max(), gx.max(), gy.max())

    return add_background(result)

def main():
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers()

    parser_mirror = subparsers.add_parser("mirror")
    parser_mirror.add_argument("axis", type=str, choices=["x", "y"])
    parser_mirror.set_defaults(convert=mirror)

    parser_rotate = subparsers.add_parser("rotate")
    parser_rotate.add_argument("turn", type=str, choices=["cw", "ccw"])
    parser_rotate.add_argument("angle", type=int)
    parser_rotate.set_defaults(convert=rotate)

    parser_sobel = subparsers.add_parser("sobel")
    parser_sobel.add_argument("axis", type=str, choices=["x", "y"])
    parser_sobel.set_defaults(convert=sobel)

    parser_median = subparsers.add_parser("median")
    parser_median.add_argument("rad", type=int)
    parser_median.set_defaults(convert=median)

    parser_gauss = subparsers.add_parser("gauss")
    parser_gauss.add_argument("sigma", type=float)
    parser_gauss.add_argument("gamma", default=2.2, type=float)
    parser_gauss.set_defaults(convert=gauss)

    parser_gradient = subparsers.add_parser("gradient")
    parser_gradient.add_argument("sigma", type=float)
    parser_gradient.set_defaults(convert=gradient)

    parser.add_argument("input_image")
    parser.add_argument("output_image")

    args = parser.parse_args()

    input_image = io.imread(args.input_image)
    image = input_image.astype(float) / 255
    output_image = args.convert(image=image, args=args)
    io.imsave(args.output_image, output_image)
