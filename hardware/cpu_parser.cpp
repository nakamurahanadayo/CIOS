#include "cpu_parser.hpp"

#include <fstream>
#include <string>

std::string CPUParser::cleanName(const std::string& raw)
{
    auto pos = raw.find(":");

    if(pos != std::string::npos)
        return raw.substr(pos + 2);

    return raw;
}

std::string CPUParser::architecture(const std::string&)
{
    return "x86_64";
}

int CPUParser::cores()
{
    std::ifstream file("/proc/cpuinfo");

    std::string line;
    int count = 0;

    while(std::getline(file, line))
    {
        if(line.find("processor") != std::string::npos)
            count++;
    }

    return count / 2;
}

int CPUParser::threads()
{
    std::ifstream file("/proc/cpuinfo");

    std::string line;
    int count = 0;

    while(std::getline(file, line))
    {
        if(line.find("processor") != std::string::npos)
            count++;
    }

    return count;
}

std::string CPUParser::frequency(const std::string& raw)
{
    auto pos = raw.find("@");

    if(pos != std::string::npos)
        return raw.substr(pos + 2);

    return "Unknown";
}

std::string CPUParser::cpuClass(const std::string& raw)
{
    if(raw.find("Xeon") != std::string::npos ||
       raw.find("EPYC") != std::string::npos)
        return "Server Performance";

    if(raw.find("i7") != std::string::npos ||
       raw.find("i9") != std::string::npos)
        return "High Performance";

    if(raw.find("i3") != std::string::npos ||
       raw.find("i5") != std::string::npos)
        return "Desktop/Mobile Performance";

    return "Legacy/Entry";
}
