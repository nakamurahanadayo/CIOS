#pragma once

#include "command.hpp"


class StartCommand : public Command
{

public:

    std::string name() override
    {
        return "/start";
    }


    std::string description() override
    {
        return "Start CIOS intelligence engine";
    }


    void execute() override;

};
