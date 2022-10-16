#pragma once
#include <memory>
#include "command.hpp"
#include "parser.hpp"

namespace game::commands {

    struct Wait: public Command<Wait>
    {
        std::size_t ticks;

        explicit Wait(std::size_t ticks) noexcept
        : ticks(ticks)
        {}
    };

    template<>
    class Parser<Wait, std::string>: public IParser<std::string>
    {
        ICommand::Ptr Parse(const std::string& data) const override;
    };

}
