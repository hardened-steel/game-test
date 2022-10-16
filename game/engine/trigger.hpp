#pragma once
#include <set>
#include <memory>

namespace game::engine {

    template<class ...Ts>
    class ITrigger
    {
    public:
        const std::size_t id;
        virtual void Action(const Ts& ...args) = 0;
    public:
        explicit ITrigger(std::size_t id) noexcept
        : id(id)
        {}
    };

    template<std::size_t Id, class ...Ts>
    class Trigger: public ITrigger<Ts...>
    {
    public:
        Trigger() noexcept
        : ITrigger<Ts...>(Id)
        {}
    };
}