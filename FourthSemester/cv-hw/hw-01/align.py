from numpy import dstack, roll, array
from skimage.transform import rescale
from math import sqrt

def move(image, rolling):
    return roll(roll(image, rolling[0], 0), rolling[1], 1)

def crop(image, border_percent):
    height, width = image.shape[:2]
    border_height = int(border_percent * height)   
    border_width = int(border_percent * width)
    return image[border_height:-border_height, border_width:-border_width]

def corr(first, second, border_percent, rolling):
    first = move(first, rolling)
    first = crop(first, border_percent)
    first = first - first.mean()
    second = crop(second, border_percent)
    second = second - second.mean()
    result = (first * second).sum() / sqrt((first ** 2).sum() * (second ** 2).sum())
    return result

def max_corr(first, second, border_percent, start=array([0, 0])):
    step = array([10, 10])
    height, width = first.shape
    if height > 200 or width > 200:
        start = max_corr(rescale(first, 0.5), rescale(second, 0.5), border_percent, start) * 2
        step = array([1, 1])
    max_corr_value = corr(first, second, border_percent, start)
    result = start
    for i in range(start[0] - step[0], start[0] + step[0] + 1):
        for j in range(start[1] - step[1], start[1] + step[1] + 1):
            val = corr(first, second, border_percent, (i, j))
            if val > max_corr_value:
                max_corr_value = val
                result = array([i, j])
    return result

def align(bgr_image):
    height, width = bgr_image.shape
    height //= 3
    border_percent = 0.07
    
    blue = bgr_image[:height]
    green = bgr_image[height:2 * height]
    red = bgr_image[2 * height:3 * height]

    red = move(red, max_corr(red, green, border_percent))
    blue = move(blue, max_corr(blue, green, border_percent))
        
    result = dstack((red, green, blue))
    result = crop(result, border_percent)
    
    return result
