#include "command_manager.hpp"
#include <iostream>

void CommandManager::registerCommand(std::unique_ptr<Command> command)
{
    commands.push_back(std::move(command));
}

void CommandManager::execute(const std::string& input)
{
    for(auto& command : commands)
    {
        if(input == command->name())
        {
            command->execute();
            return;
        }
    }

    std::cout << "Unknown command: " << input << "\n";
}

void CommandManager::showHelp()
{
    std::cout << "\nCOMMANDS\n\n";

    for(auto& command : commands)
    {
        std::cout << command->name()
                  << " - "
                  << command->description()
                  << "\n";
    }

    std::cout << "\n";
}
