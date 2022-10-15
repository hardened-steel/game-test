#include "tick.event.hpp"

namespace game::engine {

    void TickEvent::emit() const
    {
        tick.count += 1;
        IEvent<Tick>::emit(tick);
    }
}