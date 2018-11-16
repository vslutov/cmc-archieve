#pragma once

#include <utility>
#include <cstdlib>

typedef unsigned int uint;

typedef std::pair<ssize_t, ssize_t> Coord;

template <typename ValueT>
inline ValueT
sqr(const ValueT &x);

#include "util.hpp"
