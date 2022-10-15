#pragma once
#include <set>
#include <memory>

namespace game::engine {

    class Trigger
    {
    public:
        virtual void action() = 0;
    };

    template<class ...Ts>
    class ITrigger
    {
    public:
        virtual void action(Ts ...args) = 0;
    };
}