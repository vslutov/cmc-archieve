#include "model/messages.h"

#include <sstream>
#include <ctime>

void
ListenerManager::add_listener(Listener listener) {
    this->listeners.push_back(listener);
}

void
ListenerManager::send(MessageCode message_code,
                      const std::string &message_str,
                      const Image &message_image) const
{
    for (const auto &listener : this->listeners) {
        listener(message_code, message_str, message_image);
    }
}

ListenerManager::ListenerManager()
{
}

std::shared_ptr<ListenerManager>
ListenerManager::get_singleton()
{
    if (ListenerManager::singleton.use_count() == 0) {
        ListenerManager::singleton =
            std::shared_ptr<ListenerManager>(new ListenerManager());
    }
    return ListenerManager::singleton;
}

std::shared_ptr<ListenerManager> ListenerManager::singleton =
    std::shared_ptr<ListenerManager>(nullptr);

std::string
message_log(MessageCode message_code,
            const std::string &message_string,
            const Image &)
{
    std::stringstream out;

    time_t rawtime;
    time (&rawtime);
    char *str_ptr = ctime(&rawtime);
    for (ssize_t i = 0; str_ptr[i] != 0; ++ i) {
        if (str_ptr[i] == '\n') {
            str_ptr[i] = 0;
        }
    }
    out << "[";
    out << str_ptr;;
    out << "]";
    if (message_code == M_FIND_OBJ) {
        out << "Finded objects\n";
    }
    out << message_string;
    if (message_code != M_FIND_OBJ) {
        out << "\n";
    }

    return out.str();
}
