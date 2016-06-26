#pragma once

#include "matrix.h"
#include "EasyBMP.h"

#include <string>
#include <tuple>
#include <cinttypes>

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief Define usefull classes and load_image.
 */

/**
 * One pixel of image.
 */
typedef std::tuple<int16_t, int16_t, int16_t> Color;

typedef Matrix<Color> Image;

/**
 * Argument and norm of gradient.
 */
typedef std::pair<float, float> Grad;

/**
 * Any feature vector (uses for hog feature).
 */
typedef std::vector<float> Feature;

/**
 * Filename and class label.
 */
typedef std::vector<std::pair<std::string, ssize_t>> TFileList;

/**
 * Image and class label.
 */
typedef std::vector<std::pair<Image, ssize_t>> TDataSet;
typedef std::vector<Feature> TFeatures;
typedef std::vector<ssize_t> TLabels;

/**
 * Function uses EasyBMP
 */
Image
load_image(const std::string &filename);
