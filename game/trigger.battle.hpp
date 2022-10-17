#pragma once
#include <map>
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
        void Battle(engine::Map::Field field, const Warrior::Ptr& warrior_a)
        {
            if (auto it = warriors.find(field); it != warriors.end()) {
                auto& [field, warrior_p] = *it;
                auto event = BattleEvent{warrior_a, warrior_p, nullptr};
                if (warrior_p->damage < warrior_a->damage) {
                    event.winner = warrior_a;
                    march.RemoveWarrior(warrior_p);
                    warrior_p = warrior_a;
                } else if (warrior_p->damage == warrior_a->damage) {
                    march.RemoveWarrior(warrior_a);
                    march.RemoveWarrior(warrior_p);
                    warriors.erase(it);
                } else {
                    event.winner = warrior_p;
                    march.RemoveWarrior(warrior_a);
                }
                OnBattle.Emit(event);
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
            if (auto it = warriors.find(info.from); it != warriors.end()) {
                if (info.warrior == it->second) {
                    warriors.erase(it);
                }
            }
        }
        void Action(const TriggerMarch::MarchFinishEvent& info) override
        {
            Battle(info.field, info.warrior);
        }

        std::string Status() const
        {
            std::stringstream ss;
            for (const auto& [field, warrior]: warriors) {
                ss << "WARRIOR " << warrior->id << " ON " << field.x << " " << field.y << std::endl;
            }
            return ss.str();
        }
    private:
        TriggerMarch& march;
        std::map<engine::Map::Field, Warrior::Ptr> warriors;
    };
    
}
