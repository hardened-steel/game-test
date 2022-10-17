#pragma once
#include <map>
#include "trigger.battle.hpp"
#include "engine/engine.hpp"

namespace game {

    class TriggerReturn
        : public engine::Trigger<3, TriggerMarch::MarchStartEvent>
        , public engine::Trigger<3, TriggerMarch::MarchFinishEvent>
        , public engine::Trigger<3, TriggerBattle::BattleEvent>
        , public engine::Trigger<3, engine::Tick>
    {
        struct MarchInfo
        {
            engine::Map::Field from, to;
        };
    public:
        TriggerReturn(TriggerBattle& battle, TriggerMarch& march, engine::Engine& engine)
        : battle(battle), march(march), engine(engine)
        {
            engine.start.Subscribe(*this);
            engine.end.Subscribe(*this);
            battle.OnBattle.Subscribe(*this);
            march.OnMarchStarted.Subscribe(*this);
            march.OnMarchFinished.Subscribe(*this);
        }
        ~TriggerReturn() noexcept
        {
            engine.start.UnSubscribe(*this);
            engine.end.UnSubscribe(*this);
            battle.OnBattle.UnSubscribe(*this);
            march.OnMarchStarted.UnSubscribe(*this);
            march.OnMarchFinished.UnSubscribe(*this);
        }
    public:
        void Action(const engine::Tick& tick) override
        {
            if (tick == this->tick) {
                for (auto& warrior: finished) {
                    marches.erase(warrior);
                }
                finished.clear();
                for (const auto& [warrior, info]: loosers) {
                    march.March(info.to, info.from, warrior);
                }
                loosers.clear();
            } else {
                this->tick = tick;
            }
        }
        void Action(const TriggerMarch::MarchStartEvent& info) override
        {
            if (loosers.find(info.warrior) == loosers.end()) {
                marches.emplace(info.warrior, MarchInfo{info.from, info.to});
            }
        }
        void Action(const TriggerMarch::MarchFinishEvent& info) override
        {
            finished.push_back(info.warrior);
        }

        void SendLooserToHome(const Warrior::Ptr& looser)
        {
            if (auto it = marches.find(looser); it != marches.end()) {
                auto& [warrior, info] = *it;
                loosers.emplace(warrior, info);
                finished.push_back(warrior);
            }
        }
        void Action(const TriggerBattle::BattleEvent& battle) override
        {
            if (battle.winner) {
                SendLooserToHome(battle.winner == battle.warrior_a ? battle.warrior_p : battle.warrior_a);
            } else {
                SendLooserToHome(battle.warrior_a);
                SendLooserToHome(battle.warrior_p);
            }
        }
        std::string Status() const
        {
            std::stringstream ss;
            for (const auto& [warrior, info]: marches) {
                ss << "WARRIOR " << warrior->id << " MOVE FROM " << info.from.x << " " << info.from.y << " TO " << info.to.x << " " << info.to.y << std::endl;
            }
            return ss.str();
        }
    private:
        TriggerBattle& battle;
        TriggerMarch& march;
        engine::Engine& engine;
        std::map<Warrior::Ptr, MarchInfo> marches;
        std::map<Warrior::Ptr, MarchInfo> loosers;
        std::vector<Warrior::Ptr> finished;

        engine::Tick tick;
    };
    
}
