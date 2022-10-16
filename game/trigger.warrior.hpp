#pragma once
#include <map>
#include <cmath>
#include "trigger.march.hpp"

namespace game {

    class TriggerWarrior: public engine::ITrigger<engine::Map::Field, engine::Object::Ptr>
    {
    public:
        struct BattleEvent
        {
            Warrior::Ptr warrior_p;
            Warrior::Ptr warrior_a;
            Warrior::Ptr winner;
        };
    public:
        engine::IEvent<BattleEvent> OnBattle;
    public:
        explicit TriggerWarrior(TriggerMarch& march)
        : march(march)
        {
            march.OnBind.Subscribe(*this);
        }
        ~TriggerWarrior() noexcept
        {
            march.OnBind.UnSubscribe(*this);
        }
    public:
        void Battle(engine::Map::Field field, Warrior::Ptr warrior_a)
        {
            if(auto it = warriors.find(field); it != warriors.end()) {
                auto& [field, warrior_p] = *it;
                if(warrior_p->damage < warrior_a->damage) {
                    OnBattle.Emit({warrior_a, warrior_p, warrior_a});

                    march.RemoveWarrior(warrior_p);
                    warrior_p = warrior_a;
                } else if(warrior_p->damage == warrior_a->damage) {
                    OnBattle.Emit({warrior_a, warrior_p, nullptr});

                    march.RemoveWarrior(warrior_a);
                    march.RemoveWarrior(warrior_p);
                    warriors.erase(it);
                } else {
                    OnBattle.Emit({warrior_a, warrior_p, warrior_p});

                    march.RemoveWarrior(warrior_a);
                }
            } else {
                warriors[field] = warrior_a;
            }
        }
        void Action(const engine::Map::Field& field, const engine::Object::Ptr& object) override
        {
            if(object->type == Warrior::type) {
                auto warrior_a = std::static_pointer_cast<Warrior>(object);
                Battle(field, warrior_a);
            }
        }
    private:
        TriggerMarch& march;
        std::map<engine::Map::Field, Warrior::Ptr> warriors;
    };
    
}
