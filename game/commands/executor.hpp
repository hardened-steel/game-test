#pragma once
#include <set>
#include "parser.hpp"
#include "visitor.hpp"

namespace game::commands {

    template<class T>
    class Executor
    {
    public:
        using ParserPtr = typename IParser<T>::Ptr;
    public:
        template<class C, class ...TArgs>
        void AddParser(TArgs&& ...args)
        {
            parsers.insert(std::make_shared<Parser<C, T>>(std::forward<TArgs>(args)...));
        }
        const T* Process(const T& data, Visitor& visitor)
        {
            for (const auto& parser: parsers) {
                if (auto command = parser->Parse(data)) {
                    command->Accept(visitor);
                    return nullptr;
                }
            }
            return &data;
        }
    private:
        std::set<ParserPtr> parsers;
    };

}