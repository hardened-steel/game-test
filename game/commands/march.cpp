#include "march.hpp"
#include <sstream>

namespace game::commands {

    ICommand::Ptr Parser<March, std::string>::Parse(const std::string& data) const
    {
        std::stringstream ss(data);
        std::string command;
        std::size_t id;
        std::size_t x, y;
        if (ss >> command >> id >> x >> y) {
            if (command == "MARCH") {
                return std::make_unique<March>(id, x, y);
            }
        }
        return nullptr;
    }
}
