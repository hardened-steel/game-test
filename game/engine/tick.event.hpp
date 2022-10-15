#pragma once
#include "event.hpp"

namespace game::engine {

    struct Tick
    {
        std::size_t count = 0;
    };

    class TickEvent: public IEvent<Tick>
    {
    public:
        void emit() const;
    private:
        mutable Tick tick;
    };

}