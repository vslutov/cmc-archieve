#include "image.h"

#include "EasyBMP.h"

Image
load_image(const std::string &filename) {
    BMP in;
    if (!in.ReadFromFile(filename.c_str())) {
        throw "Error reading file " + filename;
    }

    Image result(in.TellHeight(), in.TellWidth());

    for (ssize_t i = 0; i < result.n_rows; ++i) {
        for (ssize_t j = 0; j < result.n_cols; ++j) {
            RGBApixel *p = in(j, i);
            result(i, j) = std::make_tuple(p->Red, p->Green, p->Blue);
        }
    }

    return result;
}
