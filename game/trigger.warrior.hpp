#pragma once
#include <map>
#include "object.warrior.hpp"
#include "engine/object.hpp"
#include "engine/tick.event.hpp"
#include "engine/map.hpp"

namespace game {

    class TriggerWarrior: public engine::ITrigger<engine::Map::Field, engine::Object::Ptr>, public engine::ITrigger<engine::Tick>
    {
        struct MarchInfo
        {
            std::size_t ticks;
            engine::Map::Field field;
        };
    public:
        struct MarchStartEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field field;
        };
        struct MarchFinishEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field field;
        };
        struct CreateWarriroEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field field;
        };
        struct BattleEvent
        {
            Warrior::Ptr warrior_p;
            Warrior::Ptr warrior_a;
            Warrior::Ptr winner;
        };
    public:
        engine::IEvent<MarchStartEvent> OnMarchStarted;
        engine::IEvent<MarchFinishEvent> OnMarchFinished;
        engine::IEvent<CreateWarriroEvent> OnWarriroCreate;
        engine::IEvent<BattleEvent> OnBattle;
    public:
        explicit TriggerWarrior(engine::Map& map) noexcept
        : map(map)
        {}
    public:
        void Battle(engine::Map::Field field, Warrior::Ptr warrior_a)
        {
            if(auto it = bindes.find(field); it != bindes.end()) {
                auto& [field, warrior_p] = *it;
                if(warrior_p->damage < warrior_a->damage) {
                    warrior_p = warrior_a;
                    warriors[warrior_a] = field;
                    warriors.erase(warrior_p);
                    OnBattle.Emit({warrior_a, warrior_p, warrior_a});
                }
                if(warrior_p->damage == warrior_a->damage) {
                    bindes.erase(it);
                    warriors.erase(warrior_p);
                    OnBattle.Emit({warrior_a, warrior_p, nullptr});
                } else {
                    OnBattle.Emit({warrior_a, warrior_p, warrior_p});
                }
            } else {
                bindes[field] = warrior_a;
                warriors[warrior_a] = field;
            }
        }
        void Action(engine::Map::Field field, engine::Object::Ptr object) override
        {
            if(object->type == Warrior::type) {
                auto warrior_a = std::static_pointer_cast<Warrior>(object);
                OnMarchFinished.Emit({warrior_a, field});
                Battle(field, warrior_a);
            }
        }
        void Action(engine::Tick tick) override
        {
            for (auto it = movings.begin(); it != movings.end();)
            {
                auto& [warrior, march] = *it;
                if (--march.ticks == 0) {
                    BindWarrior(march.field, warrior);
                    it = movings.erase(it);
                } else {
                    ++it;
                }
            }
        }
        void March(engine::Map::Field to, Warrior::Ptr warrior)
        {
            if(auto it = warriors.find(warrior); it != warriors.end()) {
                auto& field = it->second;
                bindes.erase(field);
                warriors.erase(warrior);
                movings[warrior] = MarchInfo{10, to};
                OnMarchStarted.Emit({warrior, to});
            }
        }
        void CreateWarrior(engine::Map::Field field, Warrior::Ptr warrior)
        {
            OnWarriroCreate.Emit({warrior, field});
            Battle(field, warrior);
        }
        void BindWarrior(engine::Map::Field field, Warrior::Ptr warrior)
        {
            map.BindObject(field, warrior);
        }
    private:
        engine::Map& map;
        std::map<engine::Map::Field, Warrior::Ptr> bindes;
        std::map<Warrior::Ptr, engine::Map::Field> warriors;
        std::map<Warrior::Ptr, MarchInfo> movings;
    };
    
}
