#pragma once

#include "model/color.h"
#include "model/matrix.h"
#include "model/util.h"

#include <cstddef>
#include <vector>

static const ssize_t STEP_VARS = 8;
static const ssize_t STEPS[STEP_VARS][2] = {{1, 0},
                                            {1, 1},
                                            {0, 1},
                                            {-1, 1},
                                            {-1, 0},
                                            {-1, -1},
                                            {0, -1},
                                            {1, -1}};
static const double DELTA_READIUS = 3;
static const uint INTENSITY_TRESHOLD = 65;
static const ssize_t MIN_COMPONENT = 20;
static const auto BACKGROUND = Color(0, 0, 0);

struct Component {
    static const ssize_t MIN_ROW = 0, MIN_COL = 1, MAX_ROW = 2, MAX_COL = 3;
    std::vector<ssize_t> bbox = std::vector<ssize_t>(4);
    ssize_t number, area = 0;
    Color color;
    Coord start, center = Coord(0, 0);
    double min_radius = 0, max_radius = 0;
    Matrix<bool> bin_frame = Matrix<bool>(0, 0);

    Component(ssize_t, const Coord &);

    void
    find_center(const Matrix<ssize_t> &);
};

void
dfs(const Matrix<bool> &, Matrix<ssize_t> &, const Coord &, Component &);

template <typename ValueT>
void
dfs_fill(Matrix<ValueT> &, const Coord &, const ValueT &, const ValueT &);

std::tuple<Matrix<ssize_t>, std::vector<Component>>
separate(const Matrix<bool> &m, ssize_t min_area);

Image
deseparate(const Matrix<ssize_t> &, const std::vector<Component> &);

class GetComponent {
private:
    ssize_t color;
public:
    GetComponent(ssize_t color_value);
    static const ssize_t radius = 0;

    bool
    operator() (const Matrix<ssize_t> &m) const;
};

typedef std::vector<Coord> Border;

Border
find_border(const Matrix<bool> &);

template <typename ValueT>
class Intensity {
public:
    static const ssize_t radius = 0;

    ValueT
    operator() (const Matrix<std::tuple<ValueT, ValueT, ValueT>> &) const;
};

template <typename ValueT>
class Binarizator {
private:
    ValueT treshold;
public:
    Binarizator(const ValueT &);

    static const ssize_t radius = 0;

    bool
    operator() (const Matrix<ValueT> &) const;
};

template <typename ValueT>
class Debinarizator {
private:
    ValueT fg_color, bg_color;
public:
    Debinarizator(const ValueT &, const ValueT &);

    static const ssize_t radius = 0;

    ValueT
    operator() (const Matrix<bool> &) const;
};

double
average_center_distance2(const Matrix<bool> &, const Coord &);

#include "component.hpp"
