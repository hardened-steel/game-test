#include <iostream>
#include <fstream>
#include "game/command.processor.hpp"

void RunGame(std::istream& stream)
{
    game::CommandProcessor processor;
    game::commands::Executor<std::string> executor;
    executor.AddParser<game::commands::CreateMap>();
    executor.AddParser<game::commands::Spawn>();
    executor.AddParser<game::commands::Wait>();
    executor.AddParser<game::commands::March>();
    executor.AddParser<game::commands::Finish>();
    std::string command;
    while(getline(stream, command)) {
        //std::cout << "execute command: \"" << command << "\"" << std::endl;
        executor.Process(command, processor);
    }
}

int main(int argc, char* argv[])
{
    for (auto i = 1; i < argc; ++i) {

        if(std::string_view(argv[i]) == "-") {
            RunGame(std::cin);
        } else {
            std::ifstream file(argv[i]);
            RunGame(file);
        }
    }
    return 0;
}
