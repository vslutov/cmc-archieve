#pragma once

#include "color.h"

#include <functional>
#include <memory>
#include <string>

enum MessageCode {
    M_LOAD_TASK,
    M_LOAD_GEAR,
    M_PLUGIN,
    M_ADD_BORDER,
    M_BIN,
    M_SEPARATE,
    M_FIND_OBJ,
    M_REPAIR,
    M_SAVE
};

typedef std::function<void(MessageCode, const std::string &, const Image&)>
        Listener;

class ListenerManager {
private:
    std::vector<Listener> listeners;
    static std::shared_ptr<ListenerManager> singleton;
    ListenerManager();

public:
    static std::shared_ptr<ListenerManager>
    get_singleton();

    void
    add_listener(Listener);

    void
    send(MessageCode, const std::string &, const Image &) const;
};

std::string
message_log(MessageCode, const std::string &, const Image &);
