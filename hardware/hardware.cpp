#include "hardware_manager.hpp"

#include <fstream>
#include <sstream>

std::string HardwareManager::getCPU()
{
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    while(std::getline(file,line))
    {
        if(line.find("model name") != std::string::npos)
        {
            auto pos=line.find(":");
            if(pos != std::string::npos)
                return line.substr(pos+2);
        }
    }

    return "UNKNOWN";
}

double HardwareManager::getRAMGB()
{
    std::ifstream file("/proc/meminfo");
    std::string line;

    while(std::getline(file,line))
    {
        if(line.find("MemTotal") != std::string::npos)
        {
            std::stringstream ss(line);
            std::string name;
            long long kb;

            ss >> name >> kb;

            return kb / 1024.0 / 1024.0;
        }
    }

    return 0;
}

std::string HardwareManager::getRAM()
{
    std::stringstream ss;
    ss << getRAMGB() << " GB";
    return ss.str();
}

#include <cstdio>
#include <array>

std::string HardwareManager::getGPU()
{
    std::array<char,256> buffer;
    std::string result;

    FILE* pipe = popen(
        "lspci | grep -Ei 'vga|3d|display'",
        "r"
    );

    if(!pipe)
        return "UNKNOWN";


    while(fgets(buffer.data(), buffer.size(), pipe))
    {
        result += buffer.data();
    }


    pclose(pipe);


    if(result.empty())
        return "UNKNOWN";


    if(result.back() == '\n')
        result.pop_back();


    return result;
}

std::string HardwareManager::getArchitecture()
{
    return "x86_64";
}

std::string HardwareManager::getProfile()
{
    return "PENDING";
}
