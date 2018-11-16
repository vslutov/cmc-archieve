#include "model/component.h"

#include "model/canvas.h"
#include "model/util.h"

#include <cmath>
#include <random>

static auto
random_255()
{
    static auto generator = std::default_random_engine();
    static auto distribution = std::uniform_int_distribution<uint>(0, 255);
    return distribution(generator);
}

Component::Component(ssize_t in_number, const Coord &in_start) :
    number(in_number),
    color(std::make_tuple(random_255(), random_255(), random_255())),
    start(in_start)
{
    bbox[MIN_ROW] = bbox[MAX_ROW] = start.first;
    bbox[MIN_COL] = bbox[MAX_COL] = start.second;
}

GetComponent::GetComponent(ssize_t color_value) :
    color(color_value)
{
}

bool
GetComponent::operator() (const Matrix<ssize_t> &m) const
{
    return m(0, 0) == color;
}

void
dfs(const Matrix<bool> &binar,
    Matrix<ssize_t> &result,
    const Coord &start,
    Component &c)
{
    if  (binar.in_matrix(start) &&
         binar(start.first, start.second) &&
         result(start.first, start.second) == 0) {
        result(start.first, start.second) = c.number;
        ++ c.area;

        c.bbox[Component::MIN_ROW] = std::min(c.bbox[Component::MIN_ROW], start.first);
        c.bbox[Component::MAX_ROW] = std::max(c.bbox[Component::MAX_ROW], start.first);
        c.bbox[Component::MIN_COL] = std::min(c.bbox[Component::MIN_COL], start.second);
        c.bbox[Component::MAX_COL] = std::max(c.bbox[Component::MAX_COL], start.second);

        for (ssize_t k = 0; k < STEP_VARS; ++ k) {
            dfs(binar,
                result,
                Coord(start.first + STEPS[k][0],
                      start.second + STEPS[k][1]),
                c);
        }
    }
}

std::tuple<Matrix<ssize_t>, std::vector<Component>>
separate(const Matrix<bool> &m, ssize_t min_area)
{
    Matrix<ssize_t> result(m.n_rows, m.n_cols);
    for (ssize_t i = 0; i < m.n_rows; ++ i) {
        for (ssize_t j = 0; j < m.n_cols; ++ j) {
            result(i, j) = 0;
        }
    }

    ssize_t color = 0;
    std::vector<Component> components;

    for (ssize_t i = 0; i < m.n_rows; ++ i) {
        for (ssize_t j = 0; j < m.n_cols; ++ j) {
            if (m(i, j) && result(i, j) == 0) {
                ++ color;

                auto c = Component(color, Coord(i, j));
                dfs(m, result, Coord(i, j), c);
                if (c.area < min_area) {
                    dfs_fill(result, Coord(i, j), color, static_cast<ssize_t>(-1));
                    -- color;
                } else {
                    -- c.bbox[Component::MIN_ROW];
                    -- c.bbox[Component::MIN_COL];
                    ++ c.bbox[Component::MAX_ROW];
                    ++ c.bbox[Component::MAX_COL];
                    components.push_back(c);
                }
            }
        }
    }

    for (ssize_t i = 0; i < m.n_rows; ++ i) {
        for (ssize_t j = 0; j < m.n_cols; ++ j) {
            if (result(i, j) == -1) {
                result(i, j) = 0;
            }
        }
    }

    return std::make_tuple(result, components);
}

Image
deseparate(const Matrix<ssize_t> &m, const std::vector<Component> &components)
{
    Image result(m.n_rows, m.n_cols);
    for (ssize_t i = 0; i < m.n_rows; ++ i) {
        for (ssize_t j = 0; j < m.n_cols; ++ j) {
            if (m(i, j) == 0) {
                result(i, j) = std::make_tuple(0, 0, 0);
            } else {
                result(i, j) = components[m(i, j) - 1].color;
            }
        }
    }

    return result;
}

Border
find_border(const Matrix<bool> &input_m)
{
    Border result;
    auto rows = input_m.n_rows, cols = input_m.n_cols;
    auto m = add_canvas_border(input_m, 1, false);
    for (ssize_t i = 1; i <= rows; ++ i) {
        for (ssize_t j = 1; j <= cols; ++ j) {
            if (!m(i, j)) {
                for (ssize_t k = 0; k < STEP_VARS; ++ k) {
                    if (m(i + STEPS[k][0], j + STEPS[k][1])) {
                        result.push_back(std::make_pair(i - 1, j - 1));
                        break;
                    }
                }
            }
        }
    }
    return result;
}

void
Component::find_center(const Matrix<ssize_t> &m)
{
    ssize_t rows = this->bbox[MAX_ROW] - this->bbox[MIN_ROW] + 1,
            cols = this->bbox[MAX_COL] - this->bbox[MIN_COL] + 1,
            lu_row = this->bbox[MIN_ROW],
            lu_col = this->bbox[MIN_COL];
    auto frame = m.submatrix(lu_row, lu_col, rows, cols);
    this->bin_frame = frame.unary_map(GetComponent(this->number));

    ssize_t infty = 2 * (sqr(rows) + sqr(cols));
    ssize_t min_distance2 = 0, max_distance2 = infty;
    auto border = find_border(this->bin_frame);

    for (ssize_t i = 0; i < bin_frame.n_rows; ++ i) {
        for (ssize_t j = 0; j < bin_frame.n_cols; ++ j) {
            if (bin_frame(i, j)) {
                ssize_t current_min_distance2 = infty, current_max_distance2 = 0;
                for (const auto &pixel : border) {
                    ssize_t step_distance2 = sqr(pixel.first - i) + sqr(pixel.second - j);
                    current_min_distance2 = std::min(current_min_distance2, step_distance2);
                    current_max_distance2 = std::max(current_max_distance2, step_distance2);
                }
                if (current_min_distance2 >= min_distance2) {
                    min_distance2 = current_min_distance2;
                    max_distance2 = current_max_distance2;
                    this->center = std::make_pair(i, j);
                }
            }
        }
    }

    this->min_radius = std::floor(std::sqrt(min_distance2));
    this->max_radius = std::floor(std::sqrt(max_distance2));
}

double
average_center_distance2(const Matrix<bool> &binar, const Coord &center)
{
    double row_sum = 0, col_sum = 0;
    ssize_t count = 0;
    for (ssize_t i = 0; i < binar.n_rows; ++ i) {
        for (ssize_t j = 0; j < binar.n_cols; ++ j) {
            if (binar(i, j)) {
                row_sum += i;
                col_sum += j;
                ++ count;
            }
        }
    }

    return sqr(row_sum / count - center.first) +
           sqr(col_sum / count - center.second);
}
