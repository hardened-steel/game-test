#pragma once
#include <memory>
#include "visitor.hpp"

namespace game::commands {

    class Visitor;

    class ICommand
    {
    public:
        using Ptr = std::unique_ptr<ICommand>;
        virtual void Accept(Visitor& visitor) const = 0;
    };

    template<class C>
    class Command: public ICommand
    {
        void Accept(Visitor& visitor) const override
        {
            visitor.Process(static_cast<const C&>(*this));
        }
    };

}