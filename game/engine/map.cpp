#include "map.hpp"

namespace game::engine {

    Map::Map(std::size_t h, std::size_t w)
    : H(h), W(w)
    , fields(h * w)
    {
    }

    void Map::BindObject(std::size_t h, std::size_t w, ObjectPtr object)
    {
        if((w < W) && (h < H))
        {
            auto& field = fields[h * H + w];
            field.AddObject(std::move(object));
            event.emit(field);
        }
    }

}