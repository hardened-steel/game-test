#pragma once
#include <map>
#include <cmath>
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
                    OnBattle.Emit({warrior_a, warrior_p, warrior_a});
                    warrior_p = warrior_a;
                    warriors[warrior_a] = field;
                    warriors.erase(warrior_p);
                } else if(warrior_p->damage == warrior_a->damage) {
                    OnBattle.Emit({warrior_a, warrior_p, nullptr});
                    warriors.erase(warrior_p);
                    bindes.erase(it);
                } else {
                    OnBattle.Emit({warrior_a, warrior_p, warrior_p});
                }
            } else {
                bindes[field] = warrior_a;
                warriors[warrior_a] = field;
            }
        }
        void Action(const engine::Map::Field& field, const engine::Object::Ptr& object) override
        {
            if(object->type == Warrior::type) {
                auto warrior_a = std::static_pointer_cast<Warrior>(object);
                OnMarchFinished.Emit({warrior_a, field});
                Battle(field, warrior_a);
            }
        }
        void Action(const engine::Tick& tick) override
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
            OnMarchStarted.Emit({warrior, to});
            if(auto it = warriors.find(warrior); it != warriors.end()) {
                auto& field = it->second;

                auto ticks = std::round(std::sqrt((to.x - field.x) * (to.x - field.x) + (to.y - field.y) * (to.y - field.y)));
                movings[warrior] = MarchInfo{static_cast<std::size_t>(ticks), to};

                bindes.erase(field);
                warriors.erase(it);
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
        auto MarchCount() const noexcept
        {
            return movings.size();
        }
        Warrior::Ptr FindWarrior(std::size_t id) const noexcept
        {
            for(const auto& [warrior, field]: warriors) {
                if(warrior->id == id) {
                    return warrior;
                }
            }
            return nullptr;
        }
    private:
        engine::Map& map;
        std::map<engine::Map::Field, Warrior::Ptr> bindes;
        std::map<Warrior::Ptr, engine::Map::Field> warriors;
        std::map<Warrior::Ptr, MarchInfo> movings;
    };
    
}
