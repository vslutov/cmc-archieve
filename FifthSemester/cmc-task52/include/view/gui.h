#pragma once

#include "model/color.h"
#include "model/messages.h"

#include <string>
#include <vector>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>

class Canvas : public Fl_Box {
    void draw();
    Fl_Offscreen offscreen_buffer = 0;
    Fl_Window *main_window;
    std::vector<Image> images;
    std::vector<std::string> strings;
    ssize_t image_idx = 0;
public:
    Canvas(Fl_Window *, int, int, int, int);

    void
    inc_idx();

    void
    dec_idx();

    void
    operator() (MessageCode message_code,
                const std::string &message_text,
                const Image &message_image);
};

int
gui(int argc, char **argv);
