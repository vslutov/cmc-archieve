#pragma once

#include "model/matrix.h"

#include <tuple>

typedef std::tuple<ssize_t, ssize_t, ssize_t> Color;
typedef Matrix<Color> Image;

