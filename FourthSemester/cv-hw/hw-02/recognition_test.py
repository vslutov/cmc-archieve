#!/usr/bin/python

from sys import argv, stdout, exit
from os.path import dirname
from skimage.io import imread, imsave
from recognition import generate_template, recognize

if len(argv) < 3:
    stdout.write('Usage: %s digits_dir gt_filename\n' % argv[0])
    exit(1)

digits_dir = argv[1]
gt_filename = argv[2]

digit_templates = []
for i in range(0, 10):
    digit_templates.append(generate_template(digits_dir + '/' + str(i)))

gt = []
filenames = []
for line in open(gt_filename).readlines():
    (filename, label) = line.rstrip().split(' ')
    filenames.append(filename)
    gt.append((int(label[0]), int(label[1]), int(label[2])))

testing_path = dirname(gt_filename)
accuracy = 0
for i, filename in enumerate(filenames):
    img = imread(testing_path + '/' + filename)

    digits = recognize(img, digit_templates)

    if digits == gt[i]:
        accuracy += 1

accuracy /= float(len(filenames))

stdout.write('%1.2f\n' % accuracy)


