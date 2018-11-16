#include "view/gui.h"

#include "controller/repair.h"
#include "controller/io.h"
#include "model/messages.h"
#include "model/plugins.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>

static const ssize_t W_WIDTH = 800,
                     W_HEIGHT = 600,
                     W_HALF_WIDTH = 500,
                     W_HALF_HEIGHT = 300,
                     W_BORDER = 5;

Canvas::Canvas(Fl_Window *in_window, int in_x, int in_y, int in_w, int in_h) :
    Fl_Box(in_x, in_y, in_w, in_h),
    main_window(in_window)
{
}

void
Canvas::inc_idx() {
    if (this->image_idx < static_cast<ssize_t>(this->images.size()) - 1) {
        ++ this->image_idx;
        this->redraw();
    }
}

void
Canvas::dec_idx() {
    if (this->image_idx > 0) {
        -- this->image_idx;
        this->redraw();
    }
}

void
Canvas::draw() {
    if (!offscreen_buffer) { // create the offscreen
        this->main_window->make_current(); //ensures suitable graphic context
        this->offscreen_buffer = fl_create_offscreen( w(), h() );
        if(!this->offscreen_buffer) {
            std::cerr << "Failed buffer creation" << std::endl;
            exit(1);
        }
        fl_begin_offscreen(this->offscreen_buffer); /* Open the offscreen context */
        fl_color(FL_BLACK);
        fl_rectf(0, 0, this->w(), this->h() );
        fl_end_offscreen(); /* close the offscreen context */
        /* init screen with offscreen buffer */
    }

    if (this->images.size() > 0) {
        const Image &im = this->images[this->image_idx];

        fl_begin_offscreen(this->offscreen_buffer); /* Open the offscreen context */
        fl_color(FL_BLACK);
        fl_rectf(0, 0, this->w(), this->h() );

        ssize_t zoom = std::max(std::ceil(im.n_cols * 1.0 / this->w()),
                                std::ceil(im.n_rows * 1.0 / this->h()));
        ssize_t max_i = floor(im.n_cols * 1.0 / zoom);
        ssize_t max_j = floor(im.n_rows * 1.0 / zoom);

        for (ssize_t i = 0; i < max_i; ++ i) {
            for (ssize_t j = 0; j < max_j; ++ j) {
                ssize_t r = 0, g = 0, b = 0;
                for (ssize_t sub_i = 0; sub_i < zoom; ++ sub_i) {
                    for (ssize_t sub_j = 0; sub_j < zoom; ++sub_j) {
                        ssize_t r_, g_, b_;
                        std::tie(r_, g_, b_) = im(j * zoom + sub_j, i * zoom + sub_i);
                        r += r_; g += g_; b += b_;
                    }
                }
                r /= zoom * zoom; g /= zoom * zoom; b /= zoom * zoom;
                fl_color(fl_rgb_color(r, g, b));
                fl_point(i, j);
            }
        }

        fl_color(FL_WHITE);
        fl_draw(this->strings[this->image_idx].c_str(), W_BORDER, this->h() - W_BORDER);

        fl_end_offscreen(); /* close the offscreen context */
        /* init screen with offscreen buffer */
    }

    fl_copy_offscreen(this->x(), this->y(), this->w(), this->h(), this->offscreen_buffer, 0,0);
}


void
Canvas::operator() (MessageCode message_code,
                    const std::string &message_text,
                    const Image &message_image)
{
    if (message_code != M_LOAD_GEAR) {
        this->images.push_back(message_image.deep_copy());
        if (message_code != M_FIND_OBJ) {
            this->strings.push_back(message_text);
        } else {
            this->strings.push_back("Find objects");
        }
        this->image_idx = this->images.size() - 1;
        this->redraw();
    }
}

static std::vector<Image> src_images;
static std::shared_ptr<Canvas> canvas;
static auto buff = std::shared_ptr<Fl_Text_Buffer>(new Fl_Text_Buffer());

static void
draw_image(MessageCode message_code,
           const std::string &message_text,
           const Image &message_image)
{
    (*canvas)(message_code, message_text, message_image);
}

static void
gui_log(MessageCode message_code,
        const std::string &message_string,
        const Image &message_image)
{
    buff->append(message_log(message_code, message_string, message_image).c_str());
}

static std::shared_ptr<Fl_Button>
add_button(ssize_t button_counter,
           ssize_t button_id,
           std::string button_text,
           void callback(Fl_Widget *, void *))
{
    ssize_t height = (W_HEIGHT - W_HALF_HEIGHT) / std::ceil(0.5 * button_counter);
    auto result = std::shared_ptr<Fl_Button>(
        new Fl_Button(W_BORDER + (button_id % 2) * W_HALF_WIDTH / 2,
                      W_BORDER + W_HALF_HEIGHT + button_id / 2 * height,
                      W_HALF_WIDTH / 2 - 2 * W_BORDER,
                      height - 2 * W_BORDER));
    result->copy_label(button_text.c_str());
    result->callback(callback);
    return result;
}

static void
load_callback(Fl_Widget *, void *)
{
    char *pathname = fl_file_chooser("Open task", "*.bmp", "");
    if (pathname != nullptr) {
        src_images = load_task(pathname);
    }
}

static void
prev_callback(Fl_Widget *, void *)
{
    canvas->dec_idx();
}

static void
next_callback(Fl_Widget *, void *)
{
    canvas->inc_idx();
}

static void
repair_callback(Fl_Widget *, void *)
{
    if (src_images.size() > 0) {
        repair(src_images);
    }
}

static void
exit_callback(Fl_Widget *self, void *)
{
    auto win = dynamic_cast<Fl_Window *>(self->parent());
    win->hide();
}

static void
plugin_callback(Fl_Widget *self, void *)
{
    if (src_images.size() > 0) {
        std::string plugin_name = self->label();
        src_images[0] =
            PluginManager::get_singleton()->run_plugin(plugin_name,
                                                       src_images[0]);
    }
}

static std::vector<std::shared_ptr<Fl_Button>>
add_buttons()
{
    const ssize_t DEFAULT_BUTTONS = 5;
    auto plugin_names = PluginManager::get_singleton()->plugin_names;
    ssize_t button_counter = DEFAULT_BUTTONS + plugin_names.size();
    std::vector<std::shared_ptr<Fl_Button>> result;
    result.push_back(add_button(button_counter, 0, "Prev image", prev_callback));
    result.push_back(add_button(button_counter, 1, "Next image", next_callback));
    result.push_back(add_button(button_counter, 2, "Load image", load_callback));
    result.push_back(add_button(button_counter, 3, "Repair", repair_callback));
    result.push_back(add_button(button_counter, 4, "Exit", exit_callback));
    ssize_t i = DEFAULT_BUTTONS;
    for (const auto &plugin_name : plugin_names) {
        result.push_back(add_button(button_counter,
                                    i,
                                    plugin_name, plugin_callback));
        ++ i;
    }
    return result;
}

int
gui(int argc, char **argv) {
    try {
        auto win = new Fl_Window(W_WIDTH, W_HEIGHT, "gear");
        win->begin();
            canvas = std::shared_ptr<Canvas>(new Canvas(
                win,
                W_BORDER,
                W_BORDER,
                W_HALF_WIDTH - 2 * W_BORDER,
                W_HALF_HEIGHT - 2 * W_BORDER));
            auto buttons = add_buttons();
            auto log = std::shared_ptr<Fl_Text_Display>(new Fl_Text_Display(
                W_HALF_WIDTH + W_BORDER,
                W_BORDER,
                W_WIDTH - W_HALF_WIDTH - 2 * W_BORDER,
                W_HEIGHT - 2 * W_BORDER));
            log->buffer(buff.get());
            buff->text("Log\n");

            ListenerManager::get_singleton()->add_listener(draw_image);
            ListenerManager::get_singleton()->add_listener(gui_log);
        win->end();

        win->show();
        return Fl::run();
    } catch (const std::string &s) {
        std::cerr << "Error: " << s << std::endl;
        return 1;
    }
}
