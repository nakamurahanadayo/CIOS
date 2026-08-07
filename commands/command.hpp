#pragma once
#include <string>

class Command
{
public:
    virtual std::string name() = 0;
    virtual std::string description() = 0;
    virtual void execute() = 0;
    virtual ~Command() = default;
};
