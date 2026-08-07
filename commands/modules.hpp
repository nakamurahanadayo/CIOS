#pragma once

#include "command.hpp"


class ModulesCommand : public Command
{
public:

    std::string name() override
    {
        return "/modules";
    }


    std::string description() override
    {
        return "Display installed CIOS modules";
    }


    void execute() override;
};
