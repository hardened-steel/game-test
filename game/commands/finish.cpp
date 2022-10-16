#include "finish.hpp"
#include <sstream>

namespace game::commands {

    ICommand::Ptr Parser<Finish, std::string>::Parse(const std::string& data) const
    {
        std::stringstream ss(data);
        std::string command;
        if(ss >> command) {
            if(command == "FINISH") {
                return std::make_unique<Finish>();
            }
        }
        return nullptr;
    }
}
