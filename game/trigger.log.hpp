#pragma once
#include <ostream>
#include "trigger.warrior.hpp"

namespace game {

    class TriggerLog
        : public engine::ITrigger<TriggerWarrior::MarchStartEvent>
        , public engine::ITrigger<TriggerWarrior::MarchFinishEvent>
        , public engine::ITrigger<TriggerWarrior::CreateWarriroEvent>
        , public engine::ITrigger<TriggerWarrior::BattleEvent>
        , public engine::ITrigger<engine::Tick>
    {
    public:
        explicit TriggerLog(std::ostream& stream) noexcept
        : stream(stream)
        {}
    public:
        void Action(TriggerWarrior::MarchStartEvent info) override
        {
            message += " MARCH " + std::to_string(info.warrior->id) + " TO " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
            //stream << message << std::endl;
        }
        void Action(TriggerWarrior::MarchFinishEvent info) override
        {
            message += " MARCH " + std::to_string(info.warrior->id) + " FINISHED " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
            //stream << message << std::endl;
        }
        void Action(TriggerWarrior::CreateWarriroEvent info) override
        {
            message += " WARRIOR SPAWNED " + std::to_string(info.warrior->id) + " ON " + std::to_string(info.field.x) + " " + std::to_string(info.field.y);
            //stream << message << std::endl;
        }
        void Action(TriggerWarrior::BattleEvent info) override
        {
            message += " BATTLE " + std::to_string(info.warrior_a->id) + " " + std::to_string(info.warrior_p->id);
            if(info.winner) {
                message += " WINNER IS " + std::to_string(info.winner->id);
            } else {
                message += " ALL DEAD";
            }
            //stream << message << std::endl;
        }
        void Action(engine::Tick tick) override
        {
            //stream << "[" << tick.count << "]" << "{" << this->tick.count << "}" << message << std::endl;
            if (tick == this->tick) {
                if(!message.empty()) {
                    stream << "[" << this->tick.count << "]" << message << std::endl;
                    message.clear();
                }
            } else {
                this->tick = tick;
            }
        }
    private:
        engine::Tick tick;
        std::ostream& stream;
        std::string message;
    };
    
}
