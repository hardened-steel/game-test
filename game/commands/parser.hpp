#pragma once
#include "command.hpp"

namespace game::commands {

    template<class T>
    class IParser
    {
    public:
        using Ptr = std::shared_ptr<IParser>;
        virtual ICommand::Ptr Parse(const T& data) const = 0;
    };

    template<class Command, class T>
    class Parser;

}
