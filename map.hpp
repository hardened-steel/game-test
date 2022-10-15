#pragma once
#include <memory>
#include <vector>
#include "object.hpp"
#include "event.hpp"

namespace game {

    class Object;

    class Map
    {
    public:
        using ObjectPtr = std::shared_ptr<Object>;
        class Field
        {
        public:
            void AddObject(ObjectPtr object)
            {
                objects.insert(std::move(object));
            }
            void RemoveObject(ObjectPtr object)
            {
                objects.erase(object);
            }
            const auto& GetObjects() const noexcept
            {
                return objects;
            }
        private:
            std::set<ObjectPtr> objects;
        };
        using OnBindEvent = IEvent<Field>;
    public:
        OnBindEvent event;
    public:
        Map(std::size_t w, std::size_t h);
        void BindObject(std::size_t w, std::size_t h, ObjectPtr object);
    private:
        const std::size_t W, H;
        std::vector<Field> fields;
    };

}