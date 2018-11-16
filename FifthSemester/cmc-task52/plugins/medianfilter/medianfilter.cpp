#include "model/plugins.h"
#include "model/color.h"

class MedianFilter {
public:
    ssize_t radius;
    MedianFilter(ssize_t in_radius) :
        radius(in_radius)
    {
    }

    Color
    operator()(const Image &img) const {
        ssize_t size = 2 * radius + 1;
        std::vector<ssize_t> r, g, b;
        for (ssize_t i = 0; i < size; ++ i) {
            for (ssize_t j = 0; j < size; ++ j) {
                ssize_t r_, g_, b_;
                std::tie(r_, g_, b_) = img(i, j);
                r.push_back(r_);
                g.push_back(g_);
                b.push_back(b_);
            }
        }
        std::sort(r.begin(), r.end());
        std::sort(g.begin(), g.end());
        std::sort(b.begin(), b.end());

        ssize_t med = size * size / 2;
        return std::make_tuple(r[med], g[med], b[med]);
    }
};

static Image
median_filter(const Image &img)
{
    return img.unary_map(MedianFilter(2));
}

extern "C" void
register_plugin(PluginStorage &plugin_storage)
{
    plugin_storage["Median filter"] = median_filter;
}
