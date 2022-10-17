#pragma once
#include <map>
#include <cmath>
#include <vector>
#include <system_error>
#include <sstream>
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
            std::vector<MarchFinishEvent> events;
            events.reserve(movings.size());
            for (auto it = movings.begin(); it != movings.end();)
            {
                auto& [warrior, march] = *it;
                if (march.ticks-- == 0) {
                    events.push_back({warrior, march.to});
                    BindWarrior(march.to, warrior);
                    it = movings.erase(it);
                } else {
                    ++it;
                }
            }
            for (const auto& event: events) {
                OnMarchFinished.Emit(event);
            }
        }
        std::error_code March(engine::Map::Field from, engine::Map::Field to, const Warrior::Ptr& warrior)
        {
            if ((to.x < map.H) && (to.y < map.W)) {
                if (from != to) {
                    auto ticks = std::round(std::sqrt((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)));
                    movings[warrior] = MarchInfo{static_cast<std::size_t>(ticks), from, to};

                    OnMarchStarted.Emit({warrior, from, to});
                }
                return std::error_code();
            }
            return std::make_error_code(std::errc::invalid_argument);
        }
        std::error_code March(engine::Map::Field to, const Warrior::Ptr& warrior)
        {
            if (auto it = warriors.find(warrior); it != warriors.end()) {
                auto& from = it->second;
                auto result = March(from, to, warrior);
                if (result) {
                    warriors.erase(it);
                }
                return result;
            }
            return std::make_error_code(std::errc::invalid_argument);
        }
        std::error_code March(engine::Map::Field to, std::size_t id)
        {
            if (auto warrior = FindWarrior(id)) {
                return March(to, warrior);
            }
            return std::make_error_code(std::errc::invalid_argument);
        }
        std::error_code CreateWarrior(engine::Map::Field field, const  Warrior::Ptr& warrior)
        {
            if ((field.x < map.H) && (field.y < map.W)) {
                BindWarrior(field, warrior);
                OnWarriroCreate.Emit({warrior, field});
                return std::error_code();
            }
            return std::make_error_code(std::errc::invalid_argument);
        }
        void BindWarrior(engine::Map::Field field, const Warrior::Ptr& warrior)
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
            for (const auto& [warrior, field]: warriors) {
                if (warrior->id == id) {
                    return warrior;
                }
            }
            return nullptr;
        }
        std::vector<Warrior::Ptr> GetWarriorsOnField(engine::Map::Field ifield) const
        {
            std::vector<Warrior::Ptr> result;
            for (auto& [warrior, field]: warriors) {
                if (field == ifield) {
                    result.push_back(warrior);
                }
            }
            return result;
        }
        std::string Status() const
        {
            std::stringstream ss;
            for (const auto& [warrior, field]: warriors) {
                ss << "WARRIOR " << warrior->id << " ON " << field.x << " " << field.y << std::endl;
            }
            if (!movings.empty()) {
                ss << movings.size() << " WARRIORS ON MARCH" << std::endl;
            }
            return ss.str();
        }
    private:
        engine::Map& map;
        engine::StartTickEvent& start;

        std::map<Warrior::Ptr, engine::Map::Field> warriors;
        std::map<Warrior::Ptr, MarchInfo> movings;
    };
    
}
