#pragma once

#include "image.h"

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief HOG calculate function headers.
 */

/**
 * Grayscale filter need for gradient calculating.
 */
struct Grayscale {
    /**
     * Neighbours doesn't need.
     */
    static const ssize_t vert_radius = 0;

    /**
     * Neighbours doesn't need.
     */
    static const ssize_t hor_radius = 0;

    /**
     * Used with Matrix::unary_map.
     */
    float
    operator()(const Image &im) const;
};

/**
 * Calculate norm and argument of gradient.
 * @param gray grayscale matrix of lightness
 * @param sse should we use sse?
 * @return matrix of Grad
 */
Matrix<Grad>
grad(const Matrix<float> &gray, bool sse);

/**
 * Calculate HOG pyramid.
 * @param im input image
 * @return Feature vector
 */
Feature
hog_pyramid(const Image &im);
