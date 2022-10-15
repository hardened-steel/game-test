#pragma once
#include <set>
#include <memory>

#include "trigger.hpp"

namespace game::engine {

    template<class ...Ts>
    class IEvent
    {
    public:
        using TriggerPtr = std::shared_ptr<ITrigger<Ts...>>;
    public:
        virtual void Emit(Ts ...args) const
        {
            for (const auto& trigger: triggers)
            {
                trigger->Action(args...);
            }
        }
        virtual void Subscribe(TriggerPtr trigger)
        {
            triggers.insert(std::move(trigger));
        }
        virtual void Unsubscribe(TriggerPtr trigger)
        {
            triggers.erase(trigger);
        }
    protected:
        std::set<TriggerPtr> triggers;
    };

}