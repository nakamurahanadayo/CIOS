#pragma once

#include <string>

class CPUParser
{
public:
    static std::string cleanName(const std::string& raw);
    static std::string architecture(const std::string& raw);
    static int cores();
    static int threads();
    static std::string frequency(const std::string& raw);
    static std::string cpuClass(const std::string& raw);
};
