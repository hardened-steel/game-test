#pragma once
#include "map.hpp"
#include "tick.event.hpp"

namespace game::engine {

    class Engine
    {
    public:
        using ObjectPtr = std::shared_ptr<Object>;
    public:
        StartTickEvent start;
        EndTickEvent end;
        Map map;
    public:
        Engine(std::size_t h, std::size_t w)
        : map(w, h)
        {}

        void AddObject(std::size_t x, std::size_t y, ObjectPtr object)
        {
            map.BindObject(Map::Field(x, y), std::move(object));
        }
    public: 
        auto TickStart()
        {
            return tick = start.Emit();
        }
        auto TickEnd()
        {
            return end.Emit(tick);
        }
    private:
        Tick tick;
    };

}
