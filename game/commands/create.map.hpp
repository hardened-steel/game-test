#pragma once
#include <memory>
#include "command.hpp"
#include "parser.hpp"

namespace game::commands {

    struct CreateMap: public Command<CreateMap>
    {
        std::size_t H, W;

        CreateMap(std::size_t h, std::size_t w) noexcept
        : H(h), W(w)
        {}
    };

    template<>
    class Parser<CreateMap, std::string>: public IParser<std::string>
    {
        ICommand::Ptr Parse(const std::string& data) const override;
    };

}
