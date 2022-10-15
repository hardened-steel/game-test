#pragma once
#include "map.hpp"

namespace game {

    class Warrior: public Object
    {
    public:
        explicit Warrior(std::size_t damage) noexcept
        : damage(damage)
        {}
    public:
        std::size_t damage;
    };
    
}