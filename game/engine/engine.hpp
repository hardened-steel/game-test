#pragma once
#include "map.hpp"
#include "tick.event.hpp"

namespace game::engine {

    class Engine
    {
    public:
        using ObjectPtr = std::shared_ptr<Object>;
    public:
        TickEvent event;
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
        std::size_t Tick()
        {
            return event.emit();
        }
    };

}
