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
    public:
        Engine(std::size_t h, std::size_t w)
        : map(w, h)
        {}

        void AddObject(std::size_t h, std::size_t w, ObjectPtr object)
        {
            map.BindObject(h, w, std::move(object));
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
