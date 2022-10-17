#pragma once
#include <map>
#include <cmath>
#include "trigger.battle.hpp"

namespace game {

    class TriggerReturn
        : public engine::Trigger<2, TriggerMarch::CreateWarriroEvent>
        , public engine::Trigger<2, TriggerMarch::MarchStartEvent>
        , public engine::Trigger<2, TriggerMarch::MarchFinishEvent>
        , public engine::Trigger<2, TriggerBattle::BattleEvent>
    {
    public:
        TriggerReturn(TriggerBattle& battle, TriggerMarch& march)
        : battle(battle), march(march)
        {
            battle.OnBattle.Subscribe(*this);
            march.OnMarchStarted.Subscribe(*this);
            march.OnMarchFinished.Subscribe(*this);
            march.OnWarriroCreate.Subscribe(*this);
        }
        ~TriggerReturn() noexcept
        {
            battle.OnBattle.UnSubscribe(*this);
            march.OnMarchStarted.UnSubscribe(*this);
            march.OnMarchFinished.UnSubscribe(*this);
            march.OnWarriroCreate.UnSubscribe(*this);
        }
    public:
        void Action(const TriggerMarch::CreateWarriroEvent& info) override
        {
        }
        void Action(const TriggerMarch::MarchStartEvent& info) override
        {
        }
        void Action(const TriggerMarch::MarchFinishEvent& info) override
        {
        }
        void Action(const TriggerBattle::BattleEvent& battle) override
        {
        }
    private:
        TriggerBattle& battle;
        TriggerMarch& march;
        std::map<engine::Map::Field, Warrior::Ptr> warriors;
    };
    
}
