#include <iostream>
#include <fstream>
#include "game/command.processor.hpp"

int main(int argc, char* argv[])
{
    for (auto i = 1; i < argc; ++i) {
        std::ifstream file(argv[i]);

        game::CommandProcessor processor;
        game::commands::Executor<std::string> executor;
        executor.AddParser<game::commands::CreateMap>();
        executor.AddParser<game::commands::Spawn>();
        std::string command;
        while(getline(file, command)) {
            executor.Process(command, processor);
            //std::cout << "execute command: \"" << command << "\"" << std::endl;
        }
    }
    return 0;
}
