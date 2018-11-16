#pragma once

#include "model/color.h"

#include <functional>
#include <memory>
#include <map>
#include <string>

typedef std::function<Image(const Image&)> Plugin;
typedef std::map<std::string, Plugin> PluginStorage;
typedef void (*RegisterPlugin)(PluginStorage &);

class PluginManager {
private:
    PluginStorage plugin_storage;

    static std::shared_ptr<PluginManager> singleton;
    PluginManager();
public:
    std::vector<std::string> plugin_names;

    static std::shared_ptr<PluginManager>
    get_singleton();

    Image
    run_plugin(const std::string &, const Image &);
};
