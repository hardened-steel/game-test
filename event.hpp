#pragma once
#include <set>
#include <memory>

#include "trigger.hpp"

namespace game {

    template<class ...Ts>
    class IEvent
    {
    public:
        using TriggerPtr = std::shared_ptr<ITrigger<Ts...>>;
    public:
        virtual void emit(Ts ...args) const
        {
            for (const auto& trigger: triggers)
            {
                trigger->action(args...);
            }
        }
        virtual void subscribe(TriggerPtr trigger)
        {
            triggers.insert(std::move(trigger));
        }
    protected:
        std::set<TriggerPtr> triggers;
    };

}