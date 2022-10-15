#include "tick.event.hpp"

namespace game::engine {

    std::size_t TickEvent::emit() const
    {
        tick.count += 1;
        IEvent<Tick>::Emit(tick);
        return tick.count;
    }
}