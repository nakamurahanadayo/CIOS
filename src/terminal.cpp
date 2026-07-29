#include "terminal.hpp"

#include <iostream>
#include <string>

void terminalLoop()
{
    std::string input;

    while(true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if(input == "/exit")
        {
            std::cout << "Shutting down CIOS...\n";
            break;
        }

        else if(input == "/status")
        {
            std::cout << "\nCIOS STATUS\n";
            std::cout << "Core..............ONLINE\n";
            std::cout << "Caroline..........OFFLINE\n";
            std::cout << "GLaDOS............NOT LOADED\n";
            std::cout << "Speech............OFFLINE\n";
            std::cout << "Vision............OFFLINE\n\n";
        }

        else if(input == "/help")
        {
            std::cout << "\nCommands:\n";
            std::cout << "/status - system status\n";
            std::cout << "/help   - show commands\n";
            std::cout << "/exit   - shutdown\n\n";
        }

        else
        {
            std::cout << "Command received: "
                      << input
                      << "\n";
        }
    }
}
