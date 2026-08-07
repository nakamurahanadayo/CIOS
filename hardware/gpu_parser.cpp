#include "gpu_parser.hpp"

std::string GPUParser::cleanName(const std::string& raw)
{
    if(raw.find("HD Graphics 4000") != std::string::npos)
        return "Intel HD Graphics 4000";

    if(raw.find("RTX") != std::string::npos)
        return "NVIDIA RTX GPU";

    if(raw.find("Radeon") != std::string::npos)
        return "AMD Radeon GPU";

    return "Unknown GPU";
}

std::string GPUParser::type(const std::string& raw)
{
    if(raw.find("Intel") != std::string::npos ||
       raw.find("UHD") != std::string::npos ||
       raw.find("Iris") != std::string::npos)
        return "Integrated";

    if(raw.find("RTX") != std::string::npos ||
       raw.find("Radeon") != std::string::npos)
        return "Dedicated";

    return "Unknown";
}
