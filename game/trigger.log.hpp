#pragma once
#include <ostream>
#include "trigger.warrior.hpp"
#include "commands/create.map.hpp"
#include "commands/finish.hpp"
#include "engine/engine.hpp"

namespace game {

    class TriggerLog
        : public engine::Trigger<0, TriggerMarch::MarchStartEvent>
        , public engine::Trigger<0, TriggerMarch::MarchFinishEvent>
        , public engine::Trigger<0, TriggerMarch::CreateWarriroEvent>
        , public engine::Trigger<0, TriggerWarrior::BattleEvent>
        , public engine::Trigger<0, engine::Tick>
        , public engine::Trigger<0, game::commands::CreateMap>
        , public engine::Trigger<0, game::commands::Finish>
        , public engine::Trigger<0, std::string>
    {
    public:
        TriggerLog(std::ostream& stream, TriggerMarch& marches, TriggerWarrior& warriors, engine::Engine& engine)
        : stream(stream), marches(marches), warriors(warriors), engine(engine)
        {
            marches.OnMarchStarted.Subscribe(*this);
            marches.OnMarchFinished.Subscribe(*this);
            marches.OnWarriroCreate.Subscribe(*this);
            warriors.OnBattle.Subscribe(*this);
            engine.start.Subscribe(*this);
            engine.end.Subscribe(*this);
        }
        ~TriggerLog() noexcept
        {
            marches.OnMarchStarted.UnSubscribe(*this);
            marches.OnMarchFinished.UnSubscribe(*this);
            marches.OnWarriroCreate.UnSubscribe(*this);
            warriors.OnBattle.UnSubscribe(*this);
            engine.start.UnSubscribe(*this);
            engine.end.UnSubscribe(*this);
        }
    public:
        void Action(const TriggerMarch::MarchStartEvent& info) override
        {
            message += " MARCH STARTED " + std::to_string(info.warrior->id) + " TO " + std::to_string(info.to.x) + " " + std::to_string(info.to.y);
        }
        void Action(const TriggerMarch::MarchFinishEvent& info) override
        {
            message += " MARCH " + std::to_string(info.warrior->id) + " FINISHED " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
        }
        void Action(const TriggerMarch::CreateWarriroEvent& info) override
        {
            message += " WARRIOR SPAWNED " + std::to_string(info.warrior->id) + " ON " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
        }
        void Action(const TriggerWarrior::BattleEvent& info) override
        {
            message += " BATTLE " + std::to_string(info.warrior_a->id) + " " + std::to_string(info.warrior_p->id);
            if(info.winner) {
                message += " WINNER IS " + std::to_string(info.winner->id);
            } else {
                message += " ALL DEAD";
            }
        }
        void Action(const engine::Tick& tick) override
        {
            if (tick == this->tick) {
                if(!message.empty()) {
                    stream << "[" << this->tick.count << "]" << message << std::endl;
                    message.clear();
                }
            } else {
                this->tick = tick;
            }
        }
        void Action(const game::commands::CreateMap& info) override
        {
            message += " MAP CREATED " + std::to_string(info.H) + " " + std::to_string(info.W);
        }
        void Action(const game::commands::Finish& info) override
        {
            message += " FINISH";
        }
        void Action(const std::string& info) override
        {
            message += " " + info;
        }
    private:
        std::ostream& stream;
        TriggerMarch& marches;
        TriggerWarrior& warriors;
        engine::Engine& engine;

        engine::Tick tick;
        std::string message;
    };
    
}
