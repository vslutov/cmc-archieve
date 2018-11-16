#include "model/plugins.h"
#include "model/color.h"

class BoxFilter {
public:
    ssize_t radius;
    BoxFilter(ssize_t in_radius) :
        radius(in_radius)
    {
    }

    Color
    operator()(const Image &img) const {
        ssize_t size = 2 * radius + 1;
        ssize_t r = 0, g = 0, b = 0;
        for (ssize_t i = 0; i < size; ++ i) {
            for (ssize_t j = 0; j < size; ++ j) {
                ssize_t r_, g_, b_;
                std::tie(r_, g_, b_) = img(i, j);
                r += r_;
                g += g_;
                b += b_;
            }
        }
        r /= size * size;
        g /= size * size;
        b /= size * size;

        return std::make_tuple(r, g, b);
    }
};

static Image
box_filter(const Image &img)
{
    return img.unary_map(BoxFilter(1));
}

extern "C" void
register_plugin(PluginStorage &plugin_storage)
{
    plugin_storage["Box filter"] = box_filter;
}
