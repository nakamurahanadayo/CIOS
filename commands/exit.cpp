#include "command.hpp"
#include <iostream>

class ExitCommand : public Command
{
public:
    std::string name() override { return "/exit"; }

    std::string description() override
    {
        return "shutdown CIOS";
    }

    void execute() override
    {
        std::cout << "Shutdown requested.\n";
    }
};
