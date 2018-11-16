#!/usr/bin/python3

from sys import argv, stdout, exit

from skimage.io import imread
from numpy import zeros, unique, array, where, arange, vstack, delete
from detection import train_detector, detect
from datetime import datetime

import csv
from os.path import join

def load_gt(path):
    
    
    gt_filename = join(path, 'gt.txt')
    csvfile = open(gt_filename)
    reader = csv.reader(csvfile, delimiter=';')
    next(reader)  # Remove first-line header
    
    gt = {}
    for gt_line in reader:
        filename = gt_line[0]        
        if filename not in gt:
            gt[filename] = [imread(join(path, filename)), []]
        gt[filename][1].append(gt_line[1:])
    
    for key in gt:
        gt[key][1] = array(gt[key][1]).astype(int)

    return tuple(zip(*gt.values()))

def compare_bboxes(bboxes, gt):
    new_gt = zeros([0, 4])
    for gt_i in range(gt.shape[0]):
        (x_gt_from, y_gt_from, x_gt_to, y_gt_to) = gt[gt_i, 0:4]

        for bbox_i in range(bboxes.shape[0]):
            (x_bb_from, y_bb_from, x_bb_to, y_bb_to) = bboxes[bbox_i, 0:4]

            intersection = 0
            if (min(x_bb_to, x_gt_to) > max(x_bb_from, x_gt_from) and
                min(y_bb_to, y_gt_to) > max(y_bb_from, y_gt_from)):
                    intersection = \
                        (min(x_bb_to, x_gt_to) - max(x_bb_from, x_gt_from)) * \
                        (min(y_bb_to, y_gt_to) - max(y_bb_from, y_gt_from))

            union = \
                (x_bb_to - x_bb_from) * (y_bb_to - y_bb_from) + \
                (x_gt_to - x_gt_from) * (y_gt_to - y_gt_from) - intersection;

            if intersection / float(union) >= 0.5:
                new_gt = vstack([new_gt, gt[gt_i, 0:4]])
                bboxes = delete(bboxes, bbox_i, 0)
                break

    tp = new_gt.shape[0]
    fp = bboxes.shape[0]
    fn = gt.shape[0] - tp

    return (tp, fp, fn)

def compute_roc(bboxes, gt):
    thresholds = array(arange(0, 100, 0.1))

    precision = zeros(thresholds.shape)
    recall = zeros(thresholds.shape)

    for thr_i, threshold in enumerate(thresholds):
        tp = fp = fn = 0

        for bb_i, bb in enumerate(bboxes):
            bb = bb[bb[:, 4] > threshold, :]
            (cur_tp, cur_fp, cur_fn) = compare_bboxes(bb, gt[bb_i])
            tp += cur_tp
            fp += cur_fp
            fn += cur_fn

        # Ugly hack: prevent division by zero
        if tp + fp == 0 or tp + fn == 0:
            tp = 1

        precision[thr_i] = tp / float(tp + fp)
        recall[thr_i] = tp / float(tp + fn)

    return (precision, recall)

def compute_auc(precision, recall):
    (r_values, ind) = unique(recall, return_index=True)
    p_values = zeros(r_values.shape)

    for i in range(len(p_values)):
        p_values[i] = max(precision[where(recall == r_values[i])])

    auc = sum((r_values[1:] - r_values[:-1]) * p_values[1:])
    if min(r_values) > 0:
        auc += min(r_values)

    return auc



if len(argv) != 3:
    stdout.write('Usage: %s train_dir test_dir\n' % argv[0])
    exit(1)

train_dir = argv[1]
test_dir = argv[2]


train_imgs, train_gt = load_gt(train_dir)
test_imgs, test_gt = load_gt(test_dir)

start = datetime.now()
model = train_detector(train_imgs, train_gt)
print(datetime.now() - start)
    
bboxes = []
for img in test_imgs:
    start = datetime.now()
    bboxes.append(detect(model, img))
    print(datetime.now() - start)
    
(precision, recall) = compute_roc(bboxes, test_gt)
stdout.write('%.2f\n' % compute_auc(precision, recall))
