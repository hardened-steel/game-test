#pragma once
#include <map>
#include <cmath>
#include "trigger.march.hpp"

namespace game {

    class TriggerBattle
        : public engine::Trigger<2, TriggerMarch::CreateWarriroEvent>
        , public engine::Trigger<2, TriggerMarch::MarchStartEvent>
        , public engine::Trigger<2, TriggerMarch::MarchFinishEvent>
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
        explicit TriggerBattle(TriggerMarch& march)
        : march(march)
        {
            march.OnMarchStarted.Subscribe(*this);
            march.OnMarchFinished.Subscribe(*this);
            march.OnWarriroCreate.Subscribe(*this);
        }
        ~TriggerBattle() noexcept
        {
            march.OnMarchStarted.UnSubscribe(*this);
            march.OnMarchFinished.UnSubscribe(*this);
            march.OnWarriroCreate.UnSubscribe(*this);
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
        void Action(const TriggerMarch::CreateWarriroEvent& info) override
        {
            Battle(info.field, info.warrior);
        }
        void Action(const TriggerMarch::MarchStartEvent& info) override
        {
            warriors.erase(info.from);
        }
        void Action(const TriggerMarch::MarchFinishEvent& info) override
        {
            Battle(info.field, info.warrior);
        }
    private:
        TriggerMarch& march;
        std::map<engine::Map::Field, Warrior::Ptr> warriors;
    };
    
}
