#pragma once

#include "config.hpp"
#include "command_manager.hpp"

class CIOS
{
private:

    Config config;
    CommandManager commandManager;


public:

    void start();

};
