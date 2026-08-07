#pragma once
#include "command.hpp"

class HardwareCommand : public Command
{
public:
    std::string name() override;
    std::string description() override;
    void execute() override;
};
