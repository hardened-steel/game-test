#pragma once
#include <optional>
#include "commands/executor.hpp"
#include "commands/create.map.hpp"
#include "commands/spawn.hpp"
#include "commands/march.hpp"
#include "commands/wait.hpp"
#include "commands/finish.hpp"
#include "engine/engine.hpp"
#include "object.warrior.hpp"
#include "trigger.warrior.hpp"
#include "trigger.log.hpp"

namespace game {
    class CommandProcessor: public game::commands::Visitor
    {
        engine::IEvent<game::commands::CreateMap> OnCreateMap;
        engine::IEvent<game::commands::Finish> OnFinish;
    public:
        void Process(const game::commands::CreateMap& command) override
        {
            if(!engine) {
                engine.emplace(command.H, command.W);
                warriors.emplace(engine->map);

                engine->map.event.Subscribe(*warriors);
                engine->start.Subscribe(*warriors);

                engine->start.Subscribe(log);
                engine->end.Subscribe(log);
                warriors->OnMarchStarted.Subscribe(log);
                warriors->OnMarchFinished.Subscribe(log);
                warriors->OnWarriroCreate.Subscribe(log);
                warriors->OnBattle.Subscribe(log);
                OnCreateMap.Subscribe(log);
                OnFinish.Subscribe(log);

                engine->TickStart();
                OnCreateMap.Emit(command);
                engine->TickEnd();
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
        void Process(const game::commands::March& command) override
        {
            if(engine) {
                engine->TickStart();
                if(auto warrior = warriors->FindWarrior(command.id)) {
                    warriors->March(engine::Map::Field(command.x, command.y), warrior);
                } else {
                }
                engine->TickEnd();
            } else {
            }
        }
        void Process(const game::commands::Wait& command) override
        {
            if(engine) {
                for(std::size_t i = 0; i < command.ticks; ++i) {
                    engine->TickStart();
                    engine->TickEnd();
                }
            } else {
            }
        }
        void Process(const game::commands::Finish& command) override
        {
            if(engine) {
                while(warriors->MarchCount()) {
                    engine->TickStart();
                    engine->TickEnd();
                }
                engine->TickStart();
                OnFinish.Emit(command);
                engine->TickEnd();
            } else {
            }
            
        }
    private:
        TriggerLog log {std::cout};
        std::optional<TriggerWarrior> warriors;
        std::optional<engine::Engine> engine;
    };
}