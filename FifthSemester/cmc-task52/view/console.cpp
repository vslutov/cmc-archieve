#include "view/console.h"

#include "model/plugins.h"
#include "controller/repair.h"
#include "controller/io.h"

#include <iostream>
#include <string>
#include <vector>

static void
console_log(MessageCode message_code,
            const std::string &message_string,
            const Image &message_image)
{
    std::cout << message_log(message_code, message_string, message_image);
}

class Saver {
private:
    std::string image_path, text_path;
    bool save_plugin;
public:
    Saver(const std::string &in_image_path,
          const std::string &in_text_path,
          bool in_save_plugin = false) :
        image_path(in_image_path),
        text_path(in_text_path),
        save_plugin(in_save_plugin)
    {
    }

    void
    operator() (MessageCode message_code,
                const std::string &message_text,
                const Image &message_image) const
    {
        if  (message_code == M_REPAIR ||
             (message_code == M_PLUGIN && this->save_plugin)) {

            save_image(message_image, this->image_path);

        } else if (message_code == M_FIND_OBJ) {

            std::ofstream fout(this->text_path);
            fout << message_text;

        }
    }
};

int
console(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0]
                  << " <in_image.bmp> <out_image.bmp> <out_result.txt>"
                  << std::endl

                  << "    or " << argv[0]
                  << " --plugin <in_image.bmp> <out_image.bmp>" << std::endl

                  << "    or " << argv[0] << " --gui" << std::endl;
        return 0;
    }

    auto listener_manager = ListenerManager::get_singleton();
    listener_manager->add_listener(console_log);
    try {
        if (std::string(argv[1]) == "--plugin") {
            auto src_images = load_task(argv[2]);
            listener_manager->add_listener(Saver(argv[3], "not_used.txt", true));
            auto plugin_manager = PluginManager::get_singleton();
            ssize_t plugin_count = plugin_manager->plugin_names.size();

            std::cout << "Finded " << plugin_count << " plugins." << std::endl;

            if (plugin_count > 0) {
                ssize_t i = 0;
                for (const auto &plugin_name : plugin_manager->plugin_names) {
                    ++ i;
                    std::cout << "[" << i << "] " << plugin_name << std::endl;
                }

                std::cout << "Choose plugin:" << std::endl << "> ";
                std::cin >> i;

                if (0 < i && i <= plugin_count) {
                    auto plugin_name = plugin_manager->plugin_names[i - 1];
                    plugin_manager->run_plugin(plugin_name, src_images[0]);
                } else {
                    std::cout << "Please enter number between 1 and "
                               << plugin_count << std::endl;
                }
            }
        } else {
            auto src_images = load_task(argv[1]);
            listener_manager->add_listener(Saver(argv[2], argv[3]));
            repair(src_images);
        }
    } catch (const std::string &s) {
        std::cerr << "Error: " << s << std::endl;
        return 1;
    }

    return 0;
}
