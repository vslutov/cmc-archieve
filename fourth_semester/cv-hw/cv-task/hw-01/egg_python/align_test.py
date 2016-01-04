#!/usr/bin/python

from sys import argv, stdout, exit
from os.path import basename
from glob import iglob
from align import align
from skimage.io import imread, imsave

if len(argv) != 3:
    stdout.write('Usage: %s input_dir output_dir\n' % argv[0])
    exit(1)

input_dir = argv[1]
output_dir = argv[2]

for filename in iglob(input_dir + '/*.png'):
    img = imread(filename)
    img = align(img)
    imsave(output_dir + '/' + basename(filename), img)
