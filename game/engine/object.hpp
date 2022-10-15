#pragma once
#include <cstddef>
#include <memory>

namespace game::engine {

    struct Object
    {
    public:
        using Ptr = std::shared_ptr<Object>;
    public:
        const std::size_t type;
        const std::size_t id;

        Object(std::size_t type, std::size_t id) noexcept
        : type(type), id(id)
        {}
    };

}