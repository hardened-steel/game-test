#include "map.hpp"

namespace game {

    Map::Map(std::size_t w, std::size_t h)
    : W(w), H(h)
    , fields(w * h)
    {
    }

    void Map::BindObject(std::size_t w, std::size_t h, ObjectPtr object)
    {
        if((w < W) && (h < H))
        {
            auto& field = fields[w * W + h];
            field.AddObject(std::move(object));
            event.emit(field);
        }
    }

}