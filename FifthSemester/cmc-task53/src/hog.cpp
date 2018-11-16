#include "hog.h"

#include <cinttypes>
#include <cmath>
#include <functional>
#include <mmintrin.h>

static const ssize_t CELL_ROWS = 3, CELL_COLS = 6;
static const ssize_t PYRAMID_DIV = 2;
static const ssize_t CELL_CONCAT_ROWS = 2, CELL_CONCAT_COLS = 2;
static const ssize_t SECTORS = 8;
static const float EPS = 1e-4;
static const ssize_t APPROX_POWER = 2;
static const float APPROX_STEP = 0.25;

float
Grayscale::operator()(const Image &im) const
{
    int16_t r, g, b;
    std::tie(r, g, b) = im(0, 0);
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

template<typename ValueT>
struct Convolution {
    Matrix<ValueT> matrix;
    ssize_t vert_radius, hor_radius;

    Convolution(const Matrix<ValueT> &in_matrix) :
        matrix(in_matrix.deep_copy()),
        vert_radius(in_matrix.n_rows / 2),
        hor_radius(in_matrix.n_cols / 2)
    {
    }

    ValueT
    operator()(const Matrix<ValueT> &image) const
    {
        auto result = ValueT();
        for (ssize_t i = 0; i < this->matrix.n_rows; ++ i) {
            for (ssize_t j = 0; j < this->matrix.n_cols; ++ j) {
                result += image(i, j) * this->matrix(i, j);
            }
        }
        return result;
    }
};

static const Matrix<float> sobel = {{-1, 0, 1},
                                      {-2, 0, 2},
                                      {-1, 0, 1}};

template<typename ValueT>
static inline ValueT
sqr(const ValueT &x)
{
    return x * x;
}

static inline float
my_atan2 (float y, float x) {
    float result;
    asm (
        "fpatan\n\t"
        : "=t" (result)       // outputs; t = top of fpu stack
        : "0" (x),            // inputs; 0 = same as result
        "u" (y)             //         u = 2nd floating point register
    );
    if (result >= M_PI - EPS) {
        result = -M_PI;
    }
    return result;
}

Matrix<Grad>
grad(const Matrix<float> &gray, bool sse)
{
    auto result = Matrix<Grad>(gray.n_rows, gray.n_cols);
    auto new_gray = gray.extra_borders(1, 1);

    if (sse) {
        auto pos = _mm_set_ps(1, 2, 1, 0);
        for (ssize_t i = 0; i < gray.n_rows; ++ i) {
            for (ssize_t j = 0; j < gray.n_cols; ++ j) {
                static const int MULT = 0xE1, // 1110 0001
                                 SQR = 0x51; // 0101 0001
                auto left = _mm_set_ps(new_gray(i, j),  new_gray(i + 1, j), new_gray(i + 2, j), 0);
                auto right = _mm_set_ps(new_gray(i, j + 2),  new_gray(i + 1, j + 2), new_gray(i + 2, j + 2), 0);
                auto up = _mm_set_ps(new_gray(i, j),  new_gray(i, j + 1), new_gray(i, j + 2), 0);
                auto down = _mm_set_ps(new_gray(i + 2, j),  new_gray(i + 2, j + 1), new_gray(i + 2, j + 2), 0);
                auto gx = _mm_dp_ps(_mm_sub_ps(right, left), pos, MULT);
                auto gy = _mm_dp_ps(_mm_sub_ps(down, up), pos, MULT);
                auto gxgy = _mm_shuffle_ps(gx, gy, 0); // (gx, gx, gy, gy)
                auto abs = _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(gxgy, gxgy, SQR)));
                auto arg = my_atan2(_mm_cvtss_f32(gy), _mm_cvtss_f32(gx));
                result(i, j) = std::make_pair(arg, abs);
            }
        }
    } else {
        auto gx = gray.unary_map(Convolution<float>(sobel));
        auto gy = gray.unary_map(Convolution<float>(sobel.trans()));
        for (ssize_t i = 0; i < gray.n_rows; ++ i) {
            for (ssize_t j = 0; j < gray.n_cols; ++ j) {
                result(i, j) = std::make_pair(my_atan2(gy(i, j), gx(i, j)),
                                              sqrt(sqr(gx(i, j)) + sqr(gy(i, j))));
            }
        }
    }
    return result;
}

static inline void
append(Feature &result, const Feature &appendix)
{
    result.insert(result.end(), appendix.begin(), appendix.end());
}

static inline Feature
concatenate(const Matrix<Feature> &matrix)
{
    auto result = Feature();
    for (ssize_t i = 0; i < matrix.n_rows; ++ i) {
        for (ssize_t j = 0; j < matrix.n_cols; ++ j) {
            append(result, matrix(i, j));
        }
    }
    return result;
}

static inline Feature
psi(const Feature &vector_x)
{
    auto result = Feature();
    for (const auto &x : vector_x) {
        if (x > EPS) {
            for (ssize_t i = -APPROX_POWER; i <= APPROX_POWER; ++ i) {
                float lambda = i * APPROX_STEP;
                result.push_back(sin(-lambda * log(x)) * sqrt(x / cosh(M_PI * lambda)));
                result.push_back(cos(-lambda * log(x)) * sqrt(x / cosh(M_PI * lambda)));
            }
        } else {
            for (ssize_t i = -APPROX_POWER; i <= APPROX_POWER; ++ i) {
                result.push_back(0);
                result.push_back(0);
            }
        }
    }
    return result;
}

static inline Feature
normalize(const Feature &x)
{
    auto result = Feature();
    float len = 0;
    for (const auto &elem : x) {
        len += sqr(elem);
    }
    len = sqrt(len);
    if (len > EPS) {
        for (const auto &elem : x) {
            result.push_back(elem / len);
        }
    }
    return psi(result);
}

static inline Feature
normalize(const Matrix<Feature> &matrix)
{
    return normalize(concatenate(matrix));
}

static inline Feature
hist(const Matrix<Grad> &grads)
{
    auto result = Feature(SECTORS);
    for (ssize_t i = 0; i < grads.n_rows; ++ i) {
        for (ssize_t j = 0; j < grads.n_cols; ++ j) {
            float arg = grads(i, j).first, abs = grads(i, j).second;
            ssize_t sector_num = (arg + M_PI) * 0.999 / (2 * M_PI) * SECTORS;
            if (0 <= sector_num && sector_num < SECTORS) {
                result[sector_num] += abs;
            } else {
                std::cerr << arg << ' ' << sector_num << std::endl;
            }
        }
    }
    return result;
}

static inline Matrix<Feature>
cell_descriptors(const Matrix<Grad> &grads)
{
    auto result = Matrix<Feature>(CELL_ROWS, CELL_COLS);
    ssize_t step_row = grads.n_rows / CELL_ROWS;
    ssize_t step_col = grads.n_cols / CELL_COLS;
    for (ssize_t i = 0; i < CELL_ROWS; ++ i) {
        for (ssize_t j = 0; j < CELL_COLS; ++ j) {
            result(i, j) = hist(grads.submatrix(i * step_row,
                                                j * step_col,
                                                step_row,
                                                step_col));
        }
    }
    return result;
}

static inline Feature
smart_concatenate(const Matrix<Feature> &matrix)
{
    auto result = Feature();
    for (ssize_t i = 0; i < CELL_ROWS - CELL_CONCAT_ROWS; ++ i) {
        for (ssize_t j = 0; j < CELL_COLS - CELL_CONCAT_COLS; ++ j) {
            auto part = matrix.submatrix(i, j, CELL_CONCAT_ROWS, CELL_CONCAT_COLS);
            append(result, normalize(part));
        }
    }
    return result;
}

static inline Feature
average_color(const Image &im)
{
    float r = 0, g = 0, b = 0;
    for (ssize_t i = 0; i < im.n_rows; ++ i) {
        for (ssize_t j = 0; j < im.n_cols; ++ j) {
            float r_, g_, b_;
            std::tie(r_, g_, b_) = im(i, j);
            r += r_;
            g += g_;
            b += b_;
        }
    }
    r /= 256 * im.n_rows * im.n_cols;
    g /= 256 * im.n_rows * im.n_cols;
    b /= 256 * im.n_rows * im.n_cols;
    return Feature({r, g, b});
}

static inline Feature
color_feature(const Image &im)
{
    auto result = Feature();
    ssize_t step_row = im.n_rows / CELL_ROWS;
    ssize_t step_col = im.n_cols / CELL_COLS;
    for (ssize_t i = 0; i < CELL_ROWS; ++ i) {
        for (ssize_t j = 0; j < CELL_COLS; ++ j) {
            append(result, average_color(
                im.submatrix(i * step_row, j * step_col, step_row, step_col)));
        }
    }
    return result;
}

static inline Feature
hog(const Matrix<Grad> &grads)
{
    auto descriptors = cell_descriptors(grads);
    return smart_concatenate(descriptors);
}

Feature
hog_pyramid(const Image &im)
{
    auto gray = im.unary_map(Grayscale());
    auto grads = grad(gray, true);
    auto result = Feature();

    ssize_t part_width = im.n_cols / PYRAMID_DIV;
    ssize_t part_height = im.n_rows / PYRAMID_DIV;
    for (ssize_t i = 0; i < PYRAMID_DIV; ++ i) {
        for (ssize_t j = 0; j < PYRAMID_DIV; ++ j) {
            auto part_grads = grads.submatrix(i * part_height, j * part_width,
                                              part_height, part_width);
            append(result, hog(part_grads));
        }
    }

    append(result, hog(grads));
    append(result, normalize(hist(grads)));
    append(result, color_feature(im));
    return result;
}
