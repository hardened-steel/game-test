#pragma once
#include <optional>
#include <iostream>
#include "commands/executor.hpp"
#include "commands/create.map.hpp"
#include "commands/spawn.hpp"
#include "commands/march.hpp"
#include "commands/wait.hpp"
#include "commands/finish.hpp"

#include "engine/engine.hpp"
#include "object.warrior.hpp"
#include "trigger.march.hpp"
#include "trigger.battle.hpp"
#include "trigger.return.hpp"
#include "trigger.log.hpp"

namespace game {
    class CommandProcessor: public game::commands::Visitor
    {
        class Engine: public engine::Engine
        {
        public:
            TriggerMarch marches {engine::Engine::map, engine::Engine::start};
            TriggerBattle battles {marches};
            TriggerReturn returns {battles, marches, *this};

            TriggerLog log;
        public:
            Engine(std::ostream& stream, const game::commands::CreateMap& command)
            : engine::Engine(command.H, command.W)
            , log(stream, marches, battles, *this)
            {}
        };
        engine::IEvent<game::commands::CreateMap> OnCreateMap;
        engine::IEvent<game::commands::Finish> OnFinish;
        engine::IEvent<std::string> OnError;
    public:
        void Process(const game::commands::CreateMap& command) override
        {
            if (!engine) {
                engine.emplace(std::cout, command);

                OnCreateMap.Subscribe(engine->log);
                OnFinish.Subscribe(engine->log);
                OnError.Subscribe(engine->log);

                engine->TickStart();
                OnCreateMap.Emit(command);
                engine->TickEnd();
            } else {
                std::cerr << "MAP NOT CREATED" << std::endl;
            }
        }
        void Process(const game::commands::Spawn& command) override
        {
            if (engine) {
                engine->TickStart();
                if (auto warrior = engine->marches.FindWarrior(command.id)) {
                    OnError.Emit("WARRIOR " + std::to_string((command.id)) + " ALREADY EXISTS");
                } else {
                    const engine::Map::Field field(command.x, command.y);
                    if (auto warriors = engine->marches.GetWarriorsOnField(field); warriors.empty()) {
                        if(engine->marches.CreateWarrior(field, std::make_shared<Warrior>(command.id, command.damage))) {
                            OnError.Emit("COORDINATES " + std::to_string((command.x)) + " " + std::to_string((command.y)) + " IS OUT OF MAP");
                        }
                    } else {
                        OnError.Emit("MAP " + std::to_string((command.x)) + " " + std::to_string((command.y)) + " IS BUSY");
                    }
                }
                engine->TickEnd();
            } else {
                std::cerr << "MAP NOT CREATED" << std::endl;
            }
        }
        void Process(const game::commands::March& command) override
        {
            if (engine) {
                engine->TickStart();
                if (auto warrior = engine->marches.FindWarrior(command.id)) {
                    const engine::Map::Field field(command.x, command.y);
                    if(engine->marches.March(field, warrior)) {
                        OnError.Emit("COORDINATES " + std::to_string((command.x)) + " " + std::to_string((command.y)) + " IS OUT OF MAP");
                    }
                } else {
                    OnError.Emit("WARRIOR " + std::to_string((command.id)) + " NOT FOUND");
                }
                engine->TickEnd();
            } else {
                std::cerr << "MAP NOT CREATED" << std::endl;
            }
        }
        void Process(const game::commands::Wait& command) override
        {
            if (engine) {
                for (std::size_t i = 0; i < command.ticks; ++i) {
                    engine->TickStart();
                    engine->TickEnd();
                }
            } else {
                std::cerr << "MAP NOT CREATED" << std::endl;
            }
        }
        void Process(const game::commands::Finish& command) override
        {
            if (engine) {
                while(engine->marches.MarchCount()) {
                    engine->TickStart();
                    engine->TickEnd();
                }
                engine->TickStart();
                OnFinish.Emit(command);
                engine->TickEnd();
            } else {
                std::cerr << "MAP NOT CREATED" << std::endl;
            }
        }
    private:
        std::optional<Engine> engine;
    };
}
