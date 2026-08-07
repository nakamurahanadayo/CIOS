#pragma once
#include <string>
#include <vector>

struct Module
{
    std::string name;
    std::string status;
    std::string provider;
};

class ModuleManager
{
public:
    void scan(const std::string& path);
    void showModules();

private:
    std::vector<Module> modules;
};
