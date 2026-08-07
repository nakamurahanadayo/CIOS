#include "profile.hpp"

#include <algorithm>


std::string lower(std::string text)
{
    std::transform(
        text.begin(),
                   text.end(),
                   text.begin(),
                   [](unsigned char c)
                   {
                       return std::tolower(c);
                   }
    );

    return text;
}


ProfileEngine::Rating ProfileEngine::analyze(
    const std::string& cpu,
    const std::string& gpu,
    double ramGB,
    bool hasSSD,
    bool hasCUDA
)
{
    Rating result{};


    std::string cpuData = lower(cpu);
    std::string gpuData = lower(gpu);


    // =====================
    // CPU SCORE (0-40)
    // =====================

    if(cpuData.find("xeon") != std::string::npos ||
        cpuData.find("epyc") != std::string::npos ||
        cpuData.find("threadripper") != std::string::npos)
    {
        result.cpu = 40;
    }

    else if(cpuData.find("i9") != std::string::npos ||
        cpuData.find("ryzen 9") != std::string::npos)
    {
        result.cpu = 36;
    }

    else if(cpuData.find("i7") != std::string::npos ||
        cpuData.find("ryzen 7") != std::string::npos)
    {
        result.cpu = 33;
    }

    else if(cpuData.find("i5") != std::string::npos ||
        cpuData.find("ryzen 5") != std::string::npos)
    {
        result.cpu = 28;
    }

    else if(cpuData.find("i3") != std::string::npos ||
        cpuData.find("ryzen 3") != std::string::npos)
    {
        result.cpu = 22;
    }

    else if(cpuData.find("core 2") != std::string::npos)
    {
        result.cpu = 15;
    }

    else if(cpuData.find("pentium") != std::string::npos ||
        cpuData.find("celeron") != std::string::npos)
    {
        result.cpu = 8;
    }

    else
    {
        result.cpu = 10;
    }



    // =====================
    // GPU SCORE (0-35)
    // =====================

    if(gpuData.find("rtx 4090") != std::string::npos ||
        gpuData.find("rtx 5090") != std::string::npos)
    {
        result.gpu = 35;
    }

    else if(gpuData.find("rtx") != std::string::npos)
    {
        result.gpu = 30;
    }

    else if(gpuData.find("gtx") != std::string::npos)
    {
        result.gpu = 20;
    }

    else if(gpuData.find("iris") != std::string::npos)
    {
        result.gpu = 12;
    }

    else if(
        gpuData.find("intel") != std::string::npos &&
        gpuData.find("graphics") != std::string::npos
    )
    {
        result.gpu = 5;
    }

    else
    {
        result.gpu = 3;
    }



    // =====================
    // RAM SCORE (0-15)
    // =====================

    if(ramGB >= 32)
        result.ram = 15;

    else if(ramGB >= 16)
        result.ram = 13;

    else if(ramGB >= 8)
        result.ram = 10;

    else if(ramGB >= 4)
        result.ram = 6;

    else
        result.ram = 2;



    // =====================
    // STORAGE SCORE
    // =====================

    result.storage = hasSSD ? 5 : 1;


    // =====================
    // AI FEATURES
    // =====================

    result.features = 0;

    if(hasCUDA)
        result.features += 3;



    // =====================
    // TOTAL
    // =====================

    result.total =
    result.cpu +
    result.gpu +
    result.ram +
    result.storage +
    result.features;



    // =====================
    // CLASSIFICATION
    // =====================

    if(result.total >= 81)
    {
        result.tier = "CENTRAL FUSION";
    }

    else if(result.total >= 56)
    {
        result.tier = "APEX CENTRAL";
    }

    else if(result.total >= 26)
    {
        result.tier = "EVOLUTION CORE";
    }

    else
    {
        result.tier = "GENESIS NODE";
    }


    return result;
}
