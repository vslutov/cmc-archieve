import numpy as np
from skimage import data, img_as_float
from skimage.util import random_noise
from skimage.io import imsave

np.random.seed(0)

astro = img_as_float(data.astronaut())
sigma = 0.1
noisy = random_noise(astro, var=sigma**2)

imsave('astronaut.png', noisy)
