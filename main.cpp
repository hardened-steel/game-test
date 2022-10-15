#include <iostream>
#include <game/commands/executor.hpp>
#include <game/commands/create.map.hpp>
#include <game/commands/spawn.hpp>

class CommandProcessor: public game::commands::Visitor
{
public:
    explicit CommandProcessor(std::ostream& stream) noexcept
    : stream(stream)
    {}
public:
    void Process(const game::commands::CreateMap& command) override
    {
        stream << "CREATE_MAP " << command.H << " " << command.W << std::endl;
    }
    void Process(const game::commands::Spawn& command) override
    {
        stream << "SPAWN " << command.X << " " << command.Y << std::endl;
    }
private:
    std::ostream& stream;
};

int main()
{
    CommandProcessor processor(std::cout);
    game::commands::Executor<std::string> executor;
    executor.AddParser<game::commands::CreateMap>();
    executor.AddParser<game::commands::Spawn>();
    std::string command;
    while(getline(std::cin, command)) {
        executor.Process(command, processor);
    }
    return 0;
}
