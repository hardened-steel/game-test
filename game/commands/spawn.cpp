#include "spawn.hpp"
#include <sstream>

namespace game::commands {

    ICommand::Ptr Parser<Spawn, std::string>::Parse(const std::string& data) const
    {
        std::stringstream ss(data);
        std::string command;
        std::size_t id;
        std::size_t H, W;
        std::size_t damage;
        if(ss >> command >> id >> H >> W >> damage) {
            if(command == "SPAWN") {
                return std::make_unique<Spawn>(id, H, W, damage);
            }
        }
        return nullptr;
    }
}
