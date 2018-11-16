#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import functools

from enum import Enum

import pandas as pd
import numpy as np

from skimage.io import imsave

DIRNAME = os.path.dirname(os.path.realpath(__file__))
CIE_XYZ_FNAME = 'ciexyz64.csv'
SOURCES_FNAME = 'sources.csv'

XYZ2SRGB = np.array([[ 3.2404542, -0.9692660,  0.0556434],
                     [-1.5371385,  1.8760108, -0.2040259],
                     [-0.4985314,  0.0415560,  1.0572252]])
ALPHA = 0.055
GAMMA = 1 / 2.4
THRESHOLD = 0.0031308

def xyz_to_srgb(xyz):
    xyz = xyz.copy()
    xyz[xyz < 0] = 0
    clin = xyz.dot(XYZ2SRGB)

    srgb = np.zeros(clin.shape)
    mask = clin > THRESHOLD
    srgb[mask] = (1 + ALPHA) * clin[mask] ** GAMMA - ALPHA
    srgb[np.logical_not(mask)] = 12.92 * clin[np.logical_not(mask)]

    return srgb

def get_spectral(fname):
    path = os.path.join(DIRNAME, fname)
    xyz = pd.read_csv(path, index_col=0)
    return xyz

CIE_XYZ = get_spectral(CIE_XYZ_FNAME)
SOURCES = get_spectral(SOURCES_FNAME).loc[CIE_XYZ.index]
SOURCE_CHOICES = tuple(SOURCES.columns)

def parse_args():
    parser = argparse.ArgumentParser(description='Calculate lightness.')
    parser.add_argument('-s', '--spectrum', help='Source spectral type', choices=SOURCE_CHOICES, default='D50')
    parser.add_argument('-p', '--power', help='Source power', type=int, default=5)
    parser.add_argument('-d', '--distance', help='Distance from source to platform', type=float, default=1.0)

    parser.add_argument('-r', '--resolution', help='How many subplatforms', type=int, default=501)
    parser.add_argument('-x', '--pixels', help='How many pixels in a platform', type=int, default=1)

    parser.add_argument('-o', '--output', help='Output filename', default='output.png')

    source_type = parser.add_subparsers(title='source type', dest='source_type')
    source_type.required = True

    point = source_type.add_parser('point')

    lambert = source_type.add_parser('lambert')
    lambert.add_argument('-a', '--area', help='Lambert source area', type=float, default=0.2)
    lambert.add_argument('-t', '--theta', help='Lambert source angle', type=float, default=1.5)

    args = parser.parse_args()
    return args

def read_input():
    args = parse_args()

    spectrum = SOURCES[args.spectrum].values
    spectrum = spectrum / spectrum.sum() * args.power

    return args, spectrum

def calc_point(cx, cy, I0, distance):
    r = np.sqrt(distance + cx ** 2 + cy ** 2)
    return distance * I0[np.newaxis, np.newaxis, :] / (r ** 3)[:, :, np.newaxis]

def calc_lambert(cx, cy, I0, distance, area, theta):
    right = np.zeros(cx.shape)

    N = 8
    border = np.sqrt(area)
    x1, y1 = (c.flatten() for c in np.mgrid[-border:border:1.0j * N, -border:border:1.0j * N])
    x0, y0 = cx[np.newaxis, :, :], cy[np.newaxis, :, :]

    top = ((distance + x1 * np.sin(theta))[:, np.newaxis, np.newaxis] +
           (x0 * np.sin(theta) + distance * np.cos(theta)))

    R2 = (((np.cos(theta) * x1)[:, np.newaxis, np.newaxis] - x0) ** 2 +
          (y1[:, np.newaxis, np.newaxis] - y0) ** 2 +
          ((distance + x1 * np.sin(theta)) ** 2)[:, np.newaxis, np.newaxis])
    bottom = R2 ** 2

    right = np.mean(top / bottom, axis=0)

    return I0[np.newaxis, np.newaxis, :] * right[:, :, np.newaxis]

def calc_xyz(args, spectrum):
    S = 4 * args.distance ** 2

    res = args.resolution

    if args.source_type == 'point':
        I0 = spectrum / (2 * np.pi) # Light force
    else:
        I0 = spectrum / np.pi # Light force

    I0 = I0.dot(CIE_XYZ.values)

    border = args.distance * (res - 1) / res
    cx, cy = np.mgrid[-border:border:res * 1.0j, -border:border:res * 1.0j]

    if args.source_type == 'point':
        return calc_point(cx, cy, I0, args.distance)
    else:
        return calc_lambert(cx, cy, I0, args.distance, args.area, args.theta)

def calc_output(args, spectrum):
    xyz = calc_xyz(args, spectrum)
    srgb = xyz_to_srgb(xyz)
    return srgb

def write_output(args, output):
    pixels = args.pixels

    output[output > 1] = 1
    output[output < 0] = 0
    output = (output * 255).astype(np.uint8)

    result = np.zeros((output.shape[0] * pixels, output.shape[1] * pixels, 3), dtype=np.uint8)
    for i in range(pixels):
        for j in range(pixels):
            result[i::pixels, j::pixels] = output

    imsave(args.output, result)

def main():
    args, spectrum = read_input()

    output = calc_output(args, spectrum)

    write_output(args, output)

if __name__ == "__main__":
    main()
