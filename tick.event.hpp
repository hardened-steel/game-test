#pragma once
#include "event.hpp"

namespace game {

    struct Tick
    {
        std::size_t count = 0;
    };

    class TickEvent: public IEvent<Tick>
    {
    public:
        void emit() const
        {
            tick.count += 1;
            IEvent<Tick>::emit(tick);
        }
    private:
        mutable Tick tick;
    };

}