#pragma once
#include <optional>
#include "commands/executor.hpp"
#include "commands/create.map.hpp"
#include "commands/spawn.hpp"
#include "engine/engine.hpp"
#include "object.warrior.hpp"
#include "trigger.warrior.hpp"
#include "trigger.log.hpp"

namespace game {
    class CommandProcessor: public game::commands::Visitor
    {
    public:
        void Process(const game::commands::CreateMap& command) override
        {
            if(!engine) {
                engine.emplace(command.H, command.W);
                warriors = std::make_shared<TriggerWarrior>(engine->map);
                log = std::make_shared<TriggerLog>(std::cout);
                engine->map.event.Subscribe(warriors);
                engine->start.Subscribe(warriors);
                engine->start.Subscribe(log);
                engine->end.Subscribe(log);
                warriors->OnMarchStarted.Subscribe(log);
                warriors->OnMarchFinished.Subscribe(log);
                warriors->OnWarriroCreate.Subscribe(log);
                warriors->OnBattle.Subscribe(log);
                std::cout << "[0] MAP CREATED " << command.H << " " << command.W << std::endl;
            } else {

            }
        }
        void Process(const game::commands::Spawn& command) override
        {
            if(engine) {
                engine->TickStart();
                warriors->CreateWarrior(engine::Map::Field(command.x, command.y), std::make_shared<Warrior>(command.id, command.damage));
                engine->TickEnd();
            } else {
            }
        }
    private:
        std::shared_ptr<TriggerWarrior> warriors;
        std::shared_ptr<TriggerLog> log;
        std::optional<engine::Engine> engine;
    };
}