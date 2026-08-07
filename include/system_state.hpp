#pragma once

#include <string>
#include <map>

class SystemState
{
public:
    void setModule(const std::string& name, bool online);
    bool isModuleOnline(const std::string& name);
    void showStatus();

private:
    std::map<std::string,bool> modules;
};
