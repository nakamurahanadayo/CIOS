#pragma once

#include "command.hpp"

class ShellCommand final : public Command
{
public:
    std::string name() override
    {
        return "/shell";
    }

    std::string description() override
    {
        return "Open a Linux shell";
    }

    void execute() override;
};
