#include "help.hpp"

#include <iostream>


std::string HelpCommand::name()
{
    return "/help";
}


std::string HelpCommand::description()
{
    return "show available commands";
}


void HelpCommand::execute()
{
    std::cout
    << "\nCommands:\n\n";

    std::cout
    << "/help     - show commands\n"
    << "/status   - show system status\n"
    << "/modules  - list modules\n"
    << "/model    - show intelligence model\n"
    << "/hardware - show hardware information\n"
    << "/exit     - shutdown CIOS\n\n";
}
