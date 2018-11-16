#include "glsl.h"
#include "context.h"
#include "ogl.h"
#include "draw.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int
main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " config.txt" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1]);

    if (!input.is_open()) {
        throw std::runtime_error("File " + std::string(argv[1]) + " is bad");
    }

    double M, disk_size, camera_len;
    std::string comment;
    input >> M; getline(input, comment);
    input >> disk_size; getline(input, comment);
    input >> camera_len; getline(input, comment);

    Context::setup(argc, argv);
    auto context = Context::get_singleton();
    auto program = std::make_shared<ShaderProgram>("data/shaders/blackhole.glsl");

    double G = 6.674e-11;
    double GM = G * M;
    double c = 3e8;
    double R = 2 * GM / (c * c);
    program->set_uniform("GM", GM);
    program->set_uniform("R", R);
    program->set_uniform("disk_size", disk_size);

    auto milky_way = std::make_shared<Texture>("data/textures/milky_way.png");
    auto disk = std::make_shared<Texture>("data/textures/disk.png");
    program->set_uniform("milky_way", milky_way);
    program->set_uniform("disk", disk);

    context->render_callbacks.push_back(Draw(program, camera_len * R));

    context->main_loop();
    return 0;
}

