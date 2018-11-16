#pragma once

template<typename ValueT>
Matrix<ValueT>
add_canvas_border(const Matrix<ValueT> &m,
                  ssize_t border_size,
                  const ValueT &color)
{
    auto result = Matrix<ValueT>(m.n_rows + 2 * border_size,
                                 m.n_cols + 2 * border_size);
    for (ssize_t i = 0; i < m.n_rows + 2 * border_size; ++ i) {
        for (ssize_t j = 0; j < border_size; ++ j) {
            result(i, j) = color;
            result(i, m.n_cols + border_size + j) = color;
        }
    }
    for (ssize_t j = 0; j < m.n_cols + 2 * border_size; ++ j) {
        for (ssize_t i = 0; i < border_size; ++ i) {
            result(i, j) = color;
            result(m.n_rows + border_size + i, j) = color;
        }
    }
    for (ssize_t i = 0; i < m.n_rows; ++ i) {
        for (ssize_t j = 0; j < m.n_cols; ++ j) {
            result(border_size + i, border_size + j) = m(i, j);
        }
    }

    return result;
}

template<typename ValueT>
Matrix<ValueT>
remove_canvas_border(const Matrix<ValueT> &m, ssize_t border_size)
{
    return m.submatrix(border_size,
                       border_size,
                       m.n_rows - 2 * border_size,
                       m.n_cols - 2 * border_size);
}

