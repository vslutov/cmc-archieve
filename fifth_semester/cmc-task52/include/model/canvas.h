#pragma once

template<typename ValueT>
Matrix<ValueT>
add_canvas_border(const Matrix<ValueT> &, ssize_t, const ValueT &);

template<typename ValueT>
Matrix<ValueT>
remove_canvas_border(const Matrix<ValueT> &, ssize_t);

#include "model/canvas.hpp"
