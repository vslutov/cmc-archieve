#pragma once

template <typename ValueT>
void
draw_point(Matrix<ValueT> &m,
           ssize_t row,
           ssize_t col,
           const ValueT &color)
{
    if (0 <= row && row < m.n_rows && 0 <= col && col < m.n_cols) {
        m(row, col) = color;
    }
}

template <typename ValueT>
void
draw_circle(Matrix<ValueT> &m,
            const Coord &center,
            double radius,
            const ValueT &color)
{
    ssize_t hor = floor(radius * cos(M_PI / 4));

    for (ssize_t i = -hor; i <= hor; ++ i) {
        ssize_t j = floor(sqrt(sqr(radius) - sqr(i)));
        draw_point(m, i + center.first, j + center.second, color);
        draw_point(m, i + center.first, -j + center.second, color);
        draw_point(m, j + center.first, i + center.second, color);
        draw_point(m, -j + center.first, i + center.second, color);
    }
}
