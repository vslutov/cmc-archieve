#!/bin/env python2

import sys
import os

def load_labels(filename):
    lines = open(filename, 'r').readlines()
    labels = {}
    data_path, data_filename = os.path.split(filename)
    for line in lines:
        parts = line.split()
        img_path = parts[0]
        label = parts[1]
        norm_path = os.path.abspath(os.path.join(data_path, img_path))
        labels[norm_path] = label
    return labels


def test_labels(gt_labels, predicted_labels):
    if (len(gt_labels) != len(predicted_labels)):
        print "Error! Files with predicted and ground truth labels " \
              "have different number of samples."
        return
    if (len(gt_labels) == 0):
        print "Error! Dataset is empty."
        return

    shared_items = set(gt_labels.items()) & set(predicted_labels.items())
    correct_predictions = len(shared_items)
    precision = float(correct_predictions) / len(gt_labels)
    print "Precision: %f" % precision

if len(sys.argv) != 3:
    print 'Usage: %s <ground_truth.txt> <program_output.txt>' % sys.argv[0]
    sys.exit(0)

gt_labels = load_labels(sys.argv[1])
predicted_labels = load_labels(sys.argv[2])

test_labels(gt_labels, predicted_labels)
