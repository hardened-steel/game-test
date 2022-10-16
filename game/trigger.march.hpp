#pragma once
#include <map>
#include <cmath>
#include "object.warrior.hpp"
#include "engine/object.hpp"
#include "engine/tick.event.hpp"
#include "engine/map.hpp"

namespace game {

    class TriggerMarch: public engine::Trigger<1, engine::Map::Field, engine::Object::Ptr>, public engine::Trigger<1, engine::Tick>
    {
        struct MarchInfo
        {
            std::size_t ticks;
            engine::Map::Field from, to;
        };
    public:
        struct MarchStartEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field from, to;
        };
        struct MarchFinishEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field field;
        };
        struct CreateWarriroEvent
        {
            Warrior::Ptr warrior;
            engine::Map::Field field;
        };
    public:
        engine::IEvent<MarchStartEvent> OnMarchStarted;
        engine::IEvent<MarchFinishEvent> OnMarchFinished;
        engine::IEvent<CreateWarriroEvent> OnWarriroCreate;
        engine::Map::OnBindEvent& OnBind;
    public:
        TriggerMarch(engine::Map& map, engine::StartTickEvent& start)
        : OnBind(map.event), map(map), start(start)
        {
            OnBind.Subscribe(*this);
            start.Subscribe(*this);
        }
        ~TriggerMarch() noexcept
        {
            OnBind.UnSubscribe(*this);
            start.UnSubscribe(*this);
        }
    public:
        void Action(const engine::Map::Field& field, const engine::Object::Ptr& object) override
        {
        }
        void Action(const engine::Tick& tick) override
        {
            for (auto it = movings.begin(); it != movings.end();)
            {
                auto& [warrior, march] = *it;
                if (march.ticks-- == 0) {
                    OnMarchFinished.Emit({warrior, march.to});
                    BindWarrior(march.to, warrior);
                    it = movings.erase(it);
                } else {
                    ++it;
                }
            }
        }
        void March(engine::Map::Field to, Warrior::Ptr warrior)
        {
            if(auto it = warriors.find(warrior); it != warriors.end()) {
                auto& from = it->second;

                auto ticks = std::round(std::sqrt((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)));
                movings[warrior] = MarchInfo{static_cast<std::size_t>(ticks), from, to};

                OnMarchStarted.Emit({warrior, from, to});
                warriors.erase(it);
            }
        }
        void CreateWarrior(engine::Map::Field field, Warrior::Ptr warrior)
        {
            OnWarriroCreate.Emit({warrior, field});
            BindWarrior(field, std::move(warrior));
        }
        void BindWarrior(engine::Map::Field field, Warrior::Ptr warrior)
        {
            warriors[warrior] = field;
            map.BindObject(field, warrior);
        }
        void RemoveWarrior(const Warrior::Ptr& warrior)
        {
            warriors.erase(warrior);
        }
        auto MarchCount() const noexcept
        {
            return movings.size();
        }
        Warrior::Ptr FindWarrior(std::size_t id) const noexcept
        {
            for(const auto& [warrior, field]: warriors) {
                if(warrior->id == id) {
                    return warrior;
                }
            }
            return nullptr;
        }
    private:
        engine::Map& map;
        engine::StartTickEvent& start;

        std::map<Warrior::Ptr, engine::Map::Field> warriors;
        std::map<Warrior::Ptr, MarchInfo> movings;
    };
    
}
