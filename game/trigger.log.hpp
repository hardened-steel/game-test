#pragma once
#include <ostream>
#include "trigger.warrior.hpp"
#include "commands/create.map.hpp"
#include "commands/finish.hpp"

namespace game {

    class TriggerLog
        : public engine::ITrigger<TriggerWarrior::MarchStartEvent>
        , public engine::ITrigger<TriggerWarrior::MarchFinishEvent>
        , public engine::ITrigger<TriggerWarrior::CreateWarriroEvent>
        , public engine::ITrigger<TriggerWarrior::BattleEvent>
        , public engine::ITrigger<engine::Tick>
        , public engine::ITrigger<game::commands::CreateMap>
        , public engine::ITrigger<game::commands::Finish>
    {
    public:
        explicit TriggerLog(std::ostream& stream) noexcept
        : stream(stream)
        {}
    public:
        void Action(TriggerWarrior::MarchStartEvent info) override
        {
            message += " MARCH STARTED " + std::to_string(info.warrior->id) + " TO " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
        }
        void Action(TriggerWarrior::MarchFinishEvent info) override
        {
            message += " MARCH " + std::to_string(info.warrior->id) + " FINISHED " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
        }
        void Action(TriggerWarrior::CreateWarriroEvent info) override
        {
            message += " WARRIOR SPAWNED " + std::to_string(info.warrior->id) + " ON " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
        }
        void Action(TriggerWarrior::BattleEvent info) override
        {
            message += " BATTLE " + std::to_string(info.warrior_a->id) + " " + std::to_string(info.warrior_p->id);
            if(info.winner) {
                message += " WINNER IS " + std::to_string(info.winner->id);
            } else {
                message += " ALL DEAD";
            }
        }
        void Action(engine::Tick tick) override
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
        void Action(game::commands::CreateMap info) override
        {
            message += " MAP CREATED " + std::to_string(info.H) + " " + std::to_string(info.W);
        }
        void Action(game::commands::Finish info) override
        {
            message += " FINISH";
        }
    private:
        engine::Tick tick;
        std::ostream& stream;
        std::string message;
    };
    
}
