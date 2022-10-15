#pragma once
#include <set>
#include "object.warrior.hpp"
#include "engine/tick.event.hpp"
#include "engine/map.hpp"

namespace game {

    class TriggerMarch: public engine::ITrigger<engine::Tick>
    {
    public:
        void Action(engine::Tick tick) override;
        void March(std::size_t x, std::size_t y, Warrior::Ptr warrior)
        {

        }
    private:
        engine::Map& map;
        std::set<Warrior::Ptr> warriors;
    };
    
}
