#pragma once

#include <string>

class ProfileEngine
{
public:

    struct Rating
    {
        int cpu;
        int gpu;
        int ram;
        int storage;
        int features;

        int total;
        std::string tier;
    };


    Rating analyze(
        const std::string& cpu,
        const std::string& gpu,
        double ramGB,
        bool hasSSD = false,
        bool hasCUDA = false
    );
};
