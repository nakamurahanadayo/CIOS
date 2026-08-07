#pragma once

#include <string>

class HardwareManager
{
public:
    std::string getCPU();
    std::string getRAM();
    double getRAMGB();
    std::string getGPU();
    std::string getArchitecture();
    std::string getProfile();
};
