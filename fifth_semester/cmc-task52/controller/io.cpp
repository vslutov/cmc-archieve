#include "controller/io.h"

#include "model/messages.h"
#include "EasyBMP.h"

#include <fstream>
#include <sstream>

static Image
load_image(const std::string &path)
{
    BMP in;

    if (!in.ReadFromFile(path.c_str()))
        throw std::string("Error reading file ") + path;

    Image res(in.TellHeight(), in.TellWidth());

    for (uint i = 0; i < res.n_rows; ++i) {
        for (uint j = 0; j < res.n_cols; ++j) {
            RGBApixel *p = in(j, i);
            res(i, j) = std::make_tuple(p->Red, p->Green, p->Blue);
        }
    }

    return res;
}

void save_image(const Image &im,
                const std::string &path)
{
    BMP out;
    out.SetSize(im.n_cols, im.n_rows);

    uint r, g, b;
    RGBApixel p;
    p.Alpha = 255;
    for (uint i = 0; i < im.n_rows; ++i) {
        for (uint j = 0; j < im.n_cols; ++j) {
            std::tie(r, g, b) = im(i, j);
            p.Red = r; p.Green = g; p.Blue = b;
            out.SetPixel(j, i, p);
        }
    }

    if (!out.WriteToFile(path.c_str())) {
        throw std::string("Error writing file ") + path;
    } else  {
        ListenerManager::get_singleton()->send(M_SAVE, "Saved file: " + path, im);
    }
}

std::vector<Image>
load_task(const std::string &src_imname)
{
    auto listener_manager = ListenerManager::get_singleton();
    const std::string SUFFIX = ".bmp";
    auto dot_place = src_imname.find_first_of('.');
    auto prefix = src_imname.substr(0, dot_place);

    std::vector<Image> src_images;
    auto source_image = load_image(src_imname);
    listener_manager->send(M_LOAD_TASK,
                           "Loaded source image: " + src_imname,
                           source_image);
    src_images.push_back(source_image);

    for (ssize_t i = 1; true; ++ i) {
        auto gear_name = prefix + "_" + std::to_string(i) + SUFFIX;

        std::ifstream f(gear_name.c_str());
        if (f.good()) {
            f.close();
            auto gear_image = load_image(gear_name);
            listener_manager->send(M_LOAD_GEAR,
                                   "Loaded gear: " + gear_name,
                                   gear_image);
            src_images.push_back(gear_image);
        } else {
            break;
        }
    }

    return src_images;
}

std::string
result_text(const std::vector<IObject> &object_array, ssize_t result_idx)
{
    std::stringstream fout;
    fout << result_idx << std::endl;
    fout << object_array.size() << std::endl;
    for (const auto &obj : object_array) {
        fout << obj;
    }

    return fout.str();
}
