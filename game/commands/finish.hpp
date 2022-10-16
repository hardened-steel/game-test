#pragma once
#include <memory>
#include "command.hpp"
#include "parser.hpp"

namespace game::commands {

    struct Finish: public Command<Finish>
    {
    };

    template<>
    class Parser<Finish, std::string>: public IParser<std::string>
    {
        ICommand::Ptr Parse(const std::string& data) const override;
    };

}
