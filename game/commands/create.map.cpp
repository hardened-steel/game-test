#include "create.map.hpp"
#include <sstream>

namespace game::commands {

    ICommand::Ptr Parser<CreateMap, std::string>::Parse(const std::string& data) const
    {
        std::stringstream ss(data);
        std::string command;
        std::size_t H, W;
        if (ss >> command >> H >> W) {
            if (command == "CREATE_MAP") {
                return std::make_unique<CreateMap>(H, W);
            }
        }
        return nullptr;
    }
}
