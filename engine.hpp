#pragma once
#include "map.hpp"
#include "tick.event.hpp"

namespace game {

    class Engine
    {
    public:
        using ObjectPtr = std::shared_ptr<Object>;
    public:
        TickEvent event;
    public:
        Engine(std::size_t w, std::size_t h)
        : map(w, h)
        {}

        void AddObject(std::size_t w, std::size_t h, ObjectPtr object)
        {
            map.BindObject(w, h, std::move(object));
        }
    public: 
        void tick()
        {
            event.emit();
        }
    private:
        Map map;
    };

}
