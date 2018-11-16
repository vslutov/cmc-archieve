#include "model/plugins.h"

#include "model/messages.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include <dlfcn.h>
#include <dirent.h>
#include <limits.h>
#include <string>
#include <whereami.h>

static bool
ends_with(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return fullString.compare(fullString.length() - ending.length(),
                                  ending.length(),
                                  ending) == 0;
    } else {
        return false;
    }
}

PluginManager::PluginManager()
{
    // Get execution path
    char path[PATH_MAX];
    int dirname_length = 0;
    wai_getExecutablePath(path, PATH_MAX, &dirname_length);
    path[dirname_length] = 0;
    auto plugin_path = std::string(path) + "/plugins";

    // Get plugins
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (plugin_path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            struct stat st;
            auto buf = plugin_path + "/" + std::string(ent->d_name);
            if (stat(buf.c_str(), &st) < 0) {
                continue; // !!!
            }
            if (S_ISREG(st.st_mode) && ends_with(buf, ".so")) {
                void *dl_pointer;
                if ((dl_pointer = dlopen(buf.c_str(), RTLD_NOW)) == NULL) {
                    continue;
                }
                RegisterPlugin register_plugin =
                    reinterpret_cast<RegisterPlugin>(dlsym(dl_pointer,
                                                           "register_plugin"));
                if (register_plugin == 0) {
                    continue;
                }
                register_plugin(this->plugin_storage);
            }
        }
        closedir (dir);
    }

    for (const auto &plugin : this->plugin_storage) {
        this->plugin_names.push_back(plugin.first);
    }
}

std::shared_ptr<PluginManager>
PluginManager::get_singleton()
{
    if (PluginManager::singleton.use_count() == 0) {
        PluginManager::singleton =
            std::shared_ptr<PluginManager>(new PluginManager());
    }
    return PluginManager::singleton;
}

Image
PluginManager::run_plugin(const std::string &plugin_name,
                          const Image &img)
{
    auto result = this->plugin_storage[plugin_name](img);
    ListenerManager::get_singleton()->send(M_PLUGIN,
                                           "Used plugin: " + plugin_name,
                                           result);
    return result;
}

std::shared_ptr<PluginManager> PluginManager::singleton =
    std::shared_ptr<PluginManager>(nullptr);
