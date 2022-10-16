#pragma once
#include <set>
#include <memory>

namespace game::engine {

    class Trigger
    {
    public:
        virtual void Action() = 0;
    };

    template<class ...Ts>
    class ITrigger
    {
    public:
        virtual void Action(const Ts& ...args) = 0;
    };
}