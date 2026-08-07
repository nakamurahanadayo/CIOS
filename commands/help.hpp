#pragma once

#include "command.hpp"

class HelpCommand : public Command
{
public:

    std::string name() override;

    std::string description() override;

    void execute() override;

};
