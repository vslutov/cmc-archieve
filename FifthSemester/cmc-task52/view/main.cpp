#include "view/main.h"

#include "view/console.h"
#include "view/gui.h"

#include <string>

int
main (int argc, char **argv)
{
    if (argc > 1 && std::string(argv[1]) == std::string("--gui")) {
        for (ssize_t i = 1; i < argc; ++i) {
            argv[i] = argv[i + 1];
        }
        -- argc;
        return gui(argc, argv);
    } else {
        return console(argc, argv);
    }
}
