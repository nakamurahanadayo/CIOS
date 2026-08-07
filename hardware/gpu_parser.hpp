#pragma once
#include <string>

class GPUParser
{
public:
    static std::string cleanName(const std::string& raw);
    static std::string type(const std::string& raw);
};
