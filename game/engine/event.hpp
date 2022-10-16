#pragma once
#include <set>
#include <memory>

#include "trigger.hpp"

namespace game::engine {

    template<class ...Ts>
    class IEvent
    {
    public:
        using Trigger = ITrigger<Ts...>;
    public:
        virtual void Emit(const Ts& ...args) const
        {
            for (const auto& trigger: triggers)
            {
                trigger->Action(args...);
            }
        }
        virtual void Subscribe(Trigger& trigger)
        {
            triggers.insert(&trigger);
        }
        virtual void Unsubscribe(Trigger& trigger)
        {
            triggers.erase(&trigger);
        }
    protected:
        std::set<Trigger*> triggers;
    };

}