#pragma once

#include "command.hpp"

#include <string>

class WheatleyCommand : public Command
{
public:
    std::string name() override
    {
        return "/wheatley";
    }

    std::string description() override
    {
        return "Open Wheatley personality-core interface";
    }

    void execute() override;
};
