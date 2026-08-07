#include "system_state.hpp"

#include <iostream>

void SystemState::setModule(const std::string& name, bool online)
{
    modules[name] = online;
}

bool SystemState::isModuleOnline(const std::string& name)
{
    if(modules.find(name) == modules.end())
        return false;

    return modules[name];
}

void SystemState::showStatus()
{
    std::cout << "\nCIOS STATUS\n\n";

    std::cout << "Core..............ONLINE\n";
    std::cout << "Configuration.....ONLINE\n\n";

    std::cout << "MODULES\n\n";

    for(auto& module : modules)
    {
        std::cout << module.first << "........";

        if(module.second)
            std::cout << "ONLINE\n";
        else
            std::cout << "OFFLINE\n";
    }

    std::cout << "\n";
}
