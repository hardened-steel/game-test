#pragma once
#include <memory>
#include "command.hpp"
#include "parser.hpp"

namespace game::commands {

    struct March: public Command<March>
    {
        std::size_t id;
        std::size_t x, y;

        March(std::size_t id, std::size_t x, std::size_t y) noexcept
        : id(id), x(x), y(y)
        {}
    };

    template<>
    class Parser<March, std::string>: public IParser<std::string>
    {
        ICommand::Ptr Parse(const std::string& data) const override;
    };

}
