import numpy as np
from skimage import filters, color, transform
import itertools

size_elem = np.array([8, 8])
elem_per_sizes = [8, 4]
block_size = 2
bin_counts = [8, 4]

def extract_hog_level(img, roi, elem_per_size, bin_count):
    size = elem_per_size * size_elem
    img = color.rgb2hsv(img)[:,:,2]
    
    grad = [filters.sobel_h(img), filters.sobel_v(img)]
    grad[0] = transform.resize(grad[0][roi[1]:roi[3], roi[0]:roi[2]], size)
    grad[1] = transform.resize(grad[1][roi[1]:roi[3], roi[0]:roi[2]], size)
    
    abs_grad = np.sqrt(grad[0] ** 2 + grad[1] ** 2)
    abs_grad /= np.max(abs_grad)
    
    arg_grad = np.arctan2(grad[0], grad[1])
    arg_grad = (arg_grad + np.pi) / (2 * np.pi + 0.001)
    arg_grad = (arg_grad * bin_count).astype(np.int)

    hist = np.zeros(elem_per_size ** 2 * bin_count).reshape((elem_per_size, elem_per_size, bin_count))
    
    for i in range(elem_per_size):
        for j in range(elem_per_size):
            elem_abs = abs_grad[i * size_elem[0]:(i + 1) * size_elem[0],
                                j * size_elem[1]:(j + 1) * size_elem[1]].flatten()
            elem_arg = arg_grad[i * size_elem[0]:(i + 1) * size_elem[0],
                                j * size_elem[1]:(j + 1) * size_elem[1]]
            elem_arg = elem_arg.flatten()
            
            elem_hist = np.bincount(elem_arg, elem_abs, bin_count)
            hist[i, j] = elem_hist
    
    result = []   
    
    for i in range(elem_per_size - block_size + 1):
        for j in range(elem_per_size - block_size + 1):
            block = hist[i:i + block_size, j:j + block_size].flatten()
            block /= np.sum(block) + 0.0001
            result.append(block)

    return np.concatenate(result)
    
def extract_hog(img, roi):
    result = np.concatenate([extract_hog_level(img, roi, *params) for params in itertools.product(elem_per_sizes, bin_counts)])
    return result
