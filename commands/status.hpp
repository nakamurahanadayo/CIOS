#pragma once
#include "command.hpp"

class StatusCommand : public Command
{
public:
    std::string name() override;
    std::string description() override;
    void execute() override;
};
