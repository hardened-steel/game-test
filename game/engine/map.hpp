#pragma once
#include <memory>
#include <tuple>
#include "object.hpp"
#include "event.hpp"

namespace game::engine {

    struct Object;

    class Map
    {
    public:
        class Field
        {
        public:
            Field(std::size_t x = 0, std::size_t y = 0) noexcept
            : x(x), y(y)
            {}
            std::size_t x, y;
            Field(const Field&) = default;

            friend auto GetTie(const Field& field) noexcept
            {
                return std::tie(field.x, field.y);
            }
            friend bool operator==(const Field& a, const Field& b) noexcept
            {
                return GetTie(a) == GetTie(b);
            }
            friend bool operator!=(const Field& a, const Field& b) noexcept
            {
                return GetTie(a) != GetTie(b);
            }
            friend bool operator<(const Field& a, const Field& b) noexcept
            {
                return GetTie(a) < GetTie(b);
            }
        };
        using OnBindEvent = IEvent<Field, Object::Ptr>;
    public:
        OnBindEvent event;
    public:
        Map(std::size_t h, std::size_t w) noexcept;
        void BindObject(Field field, Object::Ptr object);
        const std::size_t H, W;
    };

}