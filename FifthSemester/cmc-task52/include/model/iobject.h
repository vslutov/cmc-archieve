#pragma once

#include "model/component.h"
#include "model/util.h"

#include <cstddef>
#include <fstream>

static const ssize_t TObject = 0;
static const ssize_t TGear = 1;
static const ssize_t TAxis = 2;

struct IObject
{
    ssize_t type = TObject;

    Coord center;
    Component component;

    double min_radius, max_radius;
    bool is_broken;
    ssize_t num_cogs;

    IObject(ssize_t,
            const Coord &,
            const Component &,
            double,
            double = 0,
            bool = false,
            ssize_t = 0);
};

std::ostream &
operator<< (std::ostream &io_stream, const IObject &object);
