#include "model/iobject.h"

IObject::IObject(ssize_t in_type,
                 const Coord &in_center,
                 const Component &in_component,
                 double in_min_radius,
                 double in_max_radius,
                 bool in_is_broken,
                 ssize_t in_num_cogs) :
    type(in_type),
    center(in_center),
    component(in_component),
    min_radius(in_min_radius),
    max_radius(in_max_radius),
    is_broken(in_is_broken),
    num_cogs(in_num_cogs)
{
}

std::ostream &
operator<< (std::ostream &io_stream, const IObject &object)
{
    if (object.type == TGear) {
        io_stream << "Gear " <<
            object.center.second << " " <<
            object.center.first << " " <<
            object.max_radius << " " <<
            object.min_radius << " " <<
            (object.is_broken ? "TRUE " : "FALSE ") <<
            object.num_cogs << std::endl;
    } else {
        io_stream << "Axis " <<
        object.center.second << " " <<
        object.center.first << std::endl;
    }

    return io_stream;
}
