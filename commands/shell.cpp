#include "shell.hpp"

#include <cstdlib>
#include <iostream>

void ShellCommand::execute()
{
    std::cout << "\nEntering Linux shell.\n";
    std::cout << "Type exit to return to CIOS.\n\n";

    const int result = std::system("/bin/bash");

    if (result == -1)
    {
        std::cout << "\nFailed to open Bash.\n";
        return;
    }

    std::cout << "\nReturning to CIOS.\n";
}
