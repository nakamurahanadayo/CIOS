#pragma once

#include "command.hpp"
#include <vector>
#include <memory>
#include <string>

class CommandManager
{
public:
    void registerCommand(std::unique_ptr<Command> command);
    void execute(const std::string& input);
    void showHelp();

private:
    std::vector<std::unique_ptr<Command>> commands;
};
