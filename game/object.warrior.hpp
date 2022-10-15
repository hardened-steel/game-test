#pragma once
#include <memory>
#include "engine/object.hpp"

namespace game {

    class Warrior: public engine::Object
    {
    public:
        using Ptr = std::shared_ptr<Warrior>;
        constexpr static inline std::size_t type = 1;
    public:
        explicit Warrior(std::size_t id, std::size_t damage) noexcept
        : Object(type, id), damage(damage)
        {}
    public:
        std::size_t damage;
    };
    
}
