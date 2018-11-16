import numpy as np
from scipy import ndimage
from skimage import color, transform, filters
from sklearn.svm import LinearSVC

import random, itertools

def normalised_blocks(image):
    orientations = 8
    pixels_per_cell = (8, 8)
    cells_per_block = (2, 2)
    image = color.rgb2gray(image).astype('float')

    gx = filters.sobel_v(image)
    gy = filters.sobel_h(image)

    magnitude = np.sqrt(gx ** 2 + gy ** 2)
    orientation = np.arctan2(gy, gx) * (180 / np.pi) % 180

    sy, sx = image.shape
    cx, cy = pixels_per_cell
    bx, by = cells_per_block

    n_cellsx = int(np.floor(sx // cx))
    n_cellsy = int(np.floor(sy // cy))

    orientation_histogram = np.zeros((n_cellsy, n_cellsx, orientations))
    subsample = np.index_exp[cy // 2:cy * n_cellsy:cy,
                             cx // 2:cx * n_cellsx:cx]
    for i in range(orientations):
        cond2 = np.bitwise_and(180.0 / orientations * i <= orientation,
                               orientation < 180.0 / orientations * (i + 1))
        temp_mag = np.where(cond2, magnitude, 0)

        temp_filt = ndimage.uniform_filter(temp_mag, size=(cy, cx))
        orientation_histogram[:, :, i] = temp_filt[subsample]

    n_blocksx = (n_cellsx - bx) + 1
    n_blocksy = (n_cellsy - by) + 1
    normalised_blocks = np.zeros((n_blocksy, n_blocksx,
                                  by, bx, orientations))

    for x in range(n_blocksx):
        for y in range(n_blocksy):
            block = orientation_histogram[y:y + by, x:x + bx, :]
            eps = 1e-5
            normalised_blocks[y, x, :] = block / np.sqrt(block.sum() ** 2 + eps)

    return normalised_blocks

def extract_hog(image):
    cx, cy = (6, 6)
    pixels_per_cell = (8, 8)
    image = transform.resize(image,
                             ((cy + 1) * pixels_per_cell[1],
                              (cx + 1) * pixels_per_cell[0]))
    return normalised_blocks(image).ravel()

def detect(model, image):
    cx, cy = (6, 6)
    pixels_per_cell = (8, 8)
    
    patches = []
    
    for mx, my in itertools.product([0, pixels_per_cell[0] // 2], 
                                    [0, pixels_per_cell[1] // 2]):
        for scale in np.arange(0.3, 2.0, 0.15):
            blocks = normalised_blocks(transform.rescale(image, scale)[my:, mx:])
            
            bx, by = pixels_per_cell[0] / scale, pixels_per_cell[1] / scale
            bmx, bmy = mx / scale, my / scale
            sy, sx = blocks.shape[:2]
            rx, ry = sx - cx + 1, sy - cy + 1
            
            features = []
            for y in range(ry):
                for x in range(rx):
                    features.append(blocks[y:y + cy, x:x + cy].ravel())
        
            labels = model.predict(features).reshape((ry, rx))
            measure = model.decision_function(features).reshape((ry, rx, -1))
            
            
            for y in range(ry):
                for x in range(rx):
                    if labels[y, x] > -1:
                        patches.append([x * bx + bmx,
                                        y * by + bmy,
                                        (x + cx) * bx + bmx, 
                                        (y + cy) * by + bmy, 
                                        measure[y, x, labels[y, x] + 1]])
    
    patches.sort(key=lambda patch: patch[4], reverse=True)
    filtered_patches = []
    
    for patch in patches:
        ok = True
        for another_patch in filtered_patches:
            if is_intersect(patch, another_patch, 0.2):
                ok = False
                break
        if ok:
            filtered_patches.append(patch)
    
    patches = np.array(filtered_patches)
    
    return patches
    

def is_intersect(first, second, barier = 0.5):
    up    = max(first[1], second[1])
    down  = min(first[3], second[3])
    left  = max(first[0], second[0])
    right = min(first[2], second[2])
    
    intersect = 0
    if left <= right and up <= down: 
        intersect = (down - up) * (right - left)
    
    square_first = (first[2] - first[0]) * (first[3] - first[1])
    square_second = (second[2] - second[0]) * (second[3] - second[1])
    union = square_first + square_second - intersect
        
    return intersect / union >= barier

def in_gt(bbox, gt):
    for gt_line in gt[:,:4]:
        if is_intersect(bbox, gt_line):
            return True
    return False
        

def train_detector(imgs, gt):
    size = (56, 56)
    
    train_features = []
    train_labels = []
    for i in range(len(imgs)):
        for roi in gt[i]:
            img_roi = imgs[i][roi[1]:roi[3], roi[0]:roi[2]]
            train_features.append(extract_hog(img_roi))
            train_labels.append(roi[4])
        for j in range(5):
            random_roi = [random.randint(0, imgs[i].shape[0] - size[0]),
                          random.randint(0, imgs[i].shape[1] - size[1])]
            random_roi += [random_roi[0] + size[0], random_roi[1] + size[1]]
            
            if not in_gt(random_roi, gt[i]):
                random_roi = imgs[i][random_roi[0]:random_roi[0] + size[0],
                                 random_roi[1]:random_roi[1] + size[1]]
                train_features.append(extract_hog(random_roi))
                train_labels.append(-1)
    
    model = LinearSVC(C=0.5).fit(train_features, train_labels)
    
    
    for stage in range(100):
        for window in detect(model, imgs[stage]):
            if not in_gt(window, gt[stage]):
                wrong_roi = imgs[stage][window[1]:window[3], window[0]:window[2]]
                train_features.append(extract_hog(wrong_roi))
                train_labels.append(-1)
        
        model = LinearSVC(C=0.5).fit(train_features, train_labels)
    
    return model