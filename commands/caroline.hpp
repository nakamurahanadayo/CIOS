#pragma once

#include "command.hpp"


class CarolineCommand : public Command
{

public:

    std::string name() override
    {
        return "/glados";
    }


    std::string description() override
    {
        return "Interact with Caroline AI core";
    }


    void execute() override;

};
