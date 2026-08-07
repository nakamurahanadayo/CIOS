#pragma once

#include <string>

class LLMClient
{
public:
    std::string send(const std::string& prompt);
};
