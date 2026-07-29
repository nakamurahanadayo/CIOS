#include "cios.hpp"

#include "boot.hpp"
#include "terminal.hpp"

#include <iostream>


void CIOS::start()
{

    if(config.load("config/cios.json"))
    {
        std::cout << "Configuration loaded.\n\n";

        std::cout
        << "System: "
        << config.systemName
        << "\n";

        std::cout
        << "Base Intelligence: "
        << config.intelligence
        << "\n";

        std::cout
        << "Personality: "
        << config.personality
        << "\n\n";
    }

    else
    {
        std::cout << "Configuration failed.\n";
    }


    bootSequence();

    terminalLoop();

}
