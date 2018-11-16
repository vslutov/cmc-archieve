#pragma once

template <typename ValueT>
void
dfs_fill(Matrix<ValueT> &m,
         const Coord &start,
         const ValueT &color,
         const ValueT &filler)
{
    if  (m.in_matrix(start) &&
         m(start.first, start.second) == color) {
        m(start.first, start.second) = filler;
        for (ssize_t k = 0; k < STEP_VARS; ++ k) {
            dfs_fill(m,
                     Coord(start.first + STEPS[k][0], start.second + STEPS[k][1]),
                     color,
                     filler);
        }
    }
}

template <typename ValueT>
ValueT
Intensity<ValueT>::operator() (const Matrix<std::tuple<ValueT, ValueT, ValueT>> &m) const
{
    auto red = std::get<0>(m(0, 0));
    auto green = std::get<1>(m(0, 0));
    auto blue = std::get<2>(m(0, 0));
    return 0.299 * red + 0.587 * green + 0.114 * blue;
}

template <typename ValueT>
Binarizator<ValueT>::Binarizator(const ValueT &in_treshold) :
    treshold(in_treshold)
{
}

template <typename ValueT>
bool
Binarizator<ValueT>::operator() (const Matrix<ValueT> &m) const
{
    return m(0, 0) >= this->treshold;
}

template <typename ValueT>
Debinarizator<ValueT>::Debinarizator(const ValueT &in_fg_color, const ValueT &in_bg_color) :
    fg_color(in_fg_color),
    bg_color(in_bg_color)
{
}

template <typename ValueT>
ValueT
Debinarizator<ValueT>::operator() (const Matrix<bool> &m) const
{
    return m(0, 0) ? this->fg_color : this->bg_color;
}


