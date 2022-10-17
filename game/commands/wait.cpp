#include "wait.hpp"
#include <sstream>

namespace game::commands {

    ICommand::Ptr Parser<Wait, std::string>::Parse(const std::string& data) const
    {
        std::stringstream ss(data);
        std::string command;
        std::size_t ticks;
        if (ss >> command >> ticks) {
            if (command == "WAIT") {
                return std::make_unique<Wait>(ticks);
            }
        }
        return nullptr;
    }
}
