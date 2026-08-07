#pragma once

#include "command.hpp"

#include <string>

class AegisCommand : public Command
{
public:
    std::string name() override
    {
        return "/aegis";
    }

    std::string description() override
    {
        return "Open AEGIS facility-security interface";
    }

    void execute() override;
};
