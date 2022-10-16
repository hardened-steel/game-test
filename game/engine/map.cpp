#include "map.hpp"

namespace game::engine {

    Map::Map(std::size_t h, std::size_t w) noexcept
    : H(h), W(w)
    {
    }

    void Map::BindObject(Field field, Object::Ptr object)
    {
        if((field.x < W) && (field.y < H))
        {
            event.Emit(field, std::move(object));
        }
    }

}
