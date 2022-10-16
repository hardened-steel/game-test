#pragma once
#include "event.hpp"

namespace game::engine {

    struct Tick
    {
        std::size_t count = 0;

        friend bool operator==(const Tick& a, const Tick& b) noexcept
        {
            return a.count == b.count;
        }
        friend bool operator!=(const Tick& a, const Tick& b) noexcept
        {
            return a.count != b.count;
        }
    };

    class TickEvent: public IEvent<Tick>
    {
    public:
        std::size_t Emit() const;
    private:
        mutable Tick tick;
    };

    class StartTickEvent: public IEvent<Tick>
    {
    public:
        Tick Emit() const;
    private:
        mutable Tick tick;
    };

    class EndTickEvent: public IEvent<Tick>
    {
    };

}