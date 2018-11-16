#!/usr/bin/python3

from sys import argv, stdout, exit
from numpy import array, loadtxt, zeros
from fit_and_classify import fit_and_classify
from extract_hog import extract_hog
from skimage.io import imread

def read_gt(path):
    filename = path + '/gt.txt'
    data = loadtxt(filename, delimiter=',', skiprows=1, usecols=range(1,6))
    rois = data[:, 0:4]
    labels = data[:, 4]

    filenames = []
    for line in open(filename).readlines()[1:]:
        filenames.append(line[0:9])

    return (filenames, rois, labels)


def extract_features(path, filenames, rois):
    hog_length = len(extract_hog(imread(path + '/' + filenames[0]), rois[0, :]))
    print(rois[0, :])
    data = zeros((len(filenames), hog_length))
    for i in range(0, len(filenames)):
        filename = path + '/' + filenames[i]
        data[i, :] = extract_hog(imread(filename), rois[i, :])
    return data

if len(argv) != 3:
    stdout.write('Usage: %s train_data_path test_data_path\n' % argv[0])
    exit(1)

train_data_path = argv[1]
test_data_path = argv[2]

(train_filenames, train_rois, train_labels) = read_gt(train_data_path)
(test_filenames, test_rois, test_labels) = read_gt(test_data_path)

train_features = extract_features(train_data_path, train_filenames, train_rois)
test_features = extract_features(test_data_path, test_filenames, test_rois)

y = fit_and_classify(train_features, train_labels, test_features)
stdout.write('%.4f\n' % (sum(test_labels == y) / float(test_labels.shape[0])))


