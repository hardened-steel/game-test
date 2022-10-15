#pragma once
#include <memory>
#include "command.hpp"
#include "parser.hpp"

namespace game::commands {

    struct Spawn: public Command<Spawn>
    {
        std::size_t id;
        std::size_t X, Y;
        std::size_t damage;

        Spawn(std::size_t id, std::size_t x, std::size_t y, std::size_t damage) noexcept
        : id(id), X(x), Y(y), damage(damage)
        {}
    };

    template<>
    class Parser<Spawn, std::string>: public IParser<std::string>
    {
        ICommand::Ptr Parse(const std::string& data) const override;
    };

}
