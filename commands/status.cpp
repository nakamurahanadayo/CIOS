#include "status.hpp"
#include <iostream>

std::string StatusCommand::name()
{
    return "/status";
}

std::string StatusCommand::description()
{
    return "show full CIOS diagnostic status";
}

void StatusCommand::execute()
{
    std::cout
    << "\n===============================\n"
    << "        CIOS STATUS\n"
    << "===============================\n\n"

    << "System:\n"
    << "  Name.............CIOS\n"
    << "  Version..........0.6\n"
    << "  State............ONLINE\n\n"

    << "Core:\n"
    << "  Terminal.........ONLINE\n"
    << "  Configuration....ONLINE\n"
    << "  Command System...ONLINE\n\n"

    << "Intelligence:\n"
    << "  Base Model.......Caroline\n"
    << "  Backend..........llama.cpp\n"
    << "  Model File.......NOT LOADED\n"
    << "  Personality......GLaDOS\n"
    << "  LoRA.............NOT LOADED\n\n"

    << "Modules:\n"
    << "  Reasoning........OFFLINE\n"
    << "  Speech...........OFFLINE\n"
    << "  Vision...........OFFLINE\n"
    << "  Memory...........OFFLINE\n\n";
}
