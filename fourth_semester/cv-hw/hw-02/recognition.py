from numpy import zeros, logical_not, std
from skimage.io import imread

try:
    from skimage.filter import threshold_adaptive, gaussian_filter
except ImportError:
    from skimage.filters import threshold_adaptive, gaussian_filter

try:
    from skimage.measure import label
except ImportError:
    from skimage.morphology import label

from skimage.morphology import closing, square
from skimage.measure import regionprops
from skimage.transform import resize
from glob import glob
from math import sqrt

size = (100, 70)
border = 3

def empty_border(image, border):
    image[:border,:] = 0
    image[-border:,:] = 0
    image[:,:border] = 0
    image[:,-border:] = 0

def generate_template(digit_dir_path):
    result = zeros(size)
    filenames = glob(digit_dir_path + '/*.bmp')

    for filename in filenames:
        image = resize(imread(filename), size)
        image = logical_not(closing(threshold_adaptive(image, 80), square(2)))
        empty_border(image, border)

        result += image

    result /= len(filenames)
    return result

def is_digits(test):
    if len(test) < 3: return False
    result = 0
    width = [x.bbox[3] - x.bbox[1] for x in test]
    height = [x.bbox[2] - x.bbox[0] for x in test]
    dist = [test[1].centroid[1] - test[0].centroid[1], test[2].centroid[1] - test[1].centroid[1]]
    if max(dist) > 2 * min(width): return False
    if test[1].bbox[1] < test[0].bbox[3] or test[2].bbox[1] < test[1].bbox[3]: return False
    posy = [x.centroid[0] for x in test]
    dw = std(width)
    dh = std(height)
    dd = std(dist)
    dy = std(posy)
    result -=  dw + dd + dh + dy
    return result

def corr(first, second, digit):
    first = first - first.mean()
    second = second - second.mean()
    return (first * second).sum() / sqrt((first ** 2).sum() * (second ** 2).sum())

def recognize(image, digit_templates):
    image = gaussian_filter(image, 0.4)
    image = threshold_adaptive(image, 28, offset=0)
    label_image = label(image)

    regions = [reg for reg in regionprops(label_image) if reg.area >= 50 and 20 <= reg.bbox[2] - reg.bbox[0] <= 30]
    res = regions[:3]
    res.sort(key=lambda x: x.centroid[1])
    max_prob = is_digits(res)

    for i in range(len(regions)):
        for j in range(i + 1, len(regions)):
            for k in range(j + 1, len(regions)):
                test = [regions[i], regions[j], regions[k]]
                test.sort(key=lambda x: x.centroid[1])
                test_prob = is_digits(test)
                if not max_prob or (test_prob and test_prob > max_prob):
                    max_prob = test_prob
                    res = test

    result = [0, 2, 1]
    for i in range(len(res)):
        region = res[i]

        digit_image = resize(region.filled_image, size)
        digit = 0
        max_corr = corr(digit_image, digit_templates[digit], digit)
        for j in range(10):
            current_corr = corr(digit_image, digit_templates[j], j)
            if current_corr > max_corr:
                max_corr = current_corr
                digit = j

        result[i] = digit

    return tuple(result)
