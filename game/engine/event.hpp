#pragma once
#include <map>
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
            for (const auto& [id, trigger]: triggers)
            {
                trigger->Action(args...);
            }
        }
        virtual void Subscribe(Trigger& trigger)
        {
            triggers[trigger.id] = &trigger;
        }
        virtual void UnSubscribe(Trigger& trigger)
        {
            triggers.erase(trigger.id);
        }
    protected:
        std::map<std::size_t, Trigger*> triggers;
    };

}