#include "tick.event.hpp"

namespace game::engine {

    std::size_t TickEvent::Emit() const
    {
        tick.count += 1;
        IEvent<Tick>::Emit(tick);
        return tick.count;
    }

    Tick StartTickEvent::Emit() const
    {
        tick.count += 1;
        IEvent<Tick>::Emit(tick);
        return tick;
    }

}
