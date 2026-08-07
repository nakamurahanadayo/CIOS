#pragma once

#include "module_info.hpp"
#include <vector>

class ModuleManager
{
private:

    std::vector<ModuleInfo> modules;


public:

    void scan();

    void display();

    ModuleInfo* find(std::string name);
};
