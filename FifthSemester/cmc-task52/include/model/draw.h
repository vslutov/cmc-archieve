#pragma once

template <typename ValueT>
void
draw_point(Matrix<ValueT> &, ssize_t, ssize_t, const ValueT &);

template <typename ValueT>
void
draw_circle(Matrix<ValueT> &, const Coord &, double, const ValueT &);

#include "model/draw.hpp"
