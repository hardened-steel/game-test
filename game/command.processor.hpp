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
#include "trigger.march.hpp"
#include "trigger.warrior.hpp"
#include "trigger.log.hpp"

namespace game {
    class CommandProcessor: public game::commands::Visitor
    {
        class Engine: public engine::Engine
        {
        public:
            TriggerMarch marches {engine::Engine::map, engine::Engine::start};
            TriggerWarrior warriors {marches};

            TriggerLog log;
        public:
            Engine(std::ostream& stream, const game::commands::CreateMap& command)
            : engine::Engine(command.H, command.W)
            , log(stream, marches, warriors, *this)
            {}
        };
        engine::IEvent<game::commands::CreateMap> OnCreateMap;
        engine::IEvent<game::commands::Finish> OnFinish;
    public:
        void Process(const game::commands::CreateMap& command) override
        {
            if(!engine) {
                engine.emplace(std::cout, command);

                OnCreateMap.Subscribe(engine->log);
                OnFinish.Subscribe(engine->log);

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
                engine->marches.CreateWarrior(engine::Map::Field(command.x, command.y), std::make_shared<Warrior>(command.id, command.damage));
                engine->TickEnd();
            } else {
            }
        }
        void Process(const game::commands::March& command) override
        {
            if(engine) {
                engine->TickStart();
                if(auto warrior = engine->marches.FindWarrior(command.id)) {
                    engine->marches.March(engine::Map::Field(command.x, command.y), warrior);
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
                while(engine->marches.MarchCount()) {
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
        std::optional<Engine> engine;
    };
}
