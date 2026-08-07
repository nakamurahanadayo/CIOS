#include "hardware.hpp"
#include "../hardware/gpu_parser.hpp"
#include "../hardware/hardware_manager.hpp"
#include "../hardware/profile.hpp"
#include "../hardware/cpu_parser.hpp"
#include <iostream>

std::string HardwareCommand::name()
{
    return "/hardware";
}


std::string HardwareCommand::description()
{
    return "show hardware information";
}


void HardwareCommand::execute()
{
    HardwareManager hardware;
    ProfileEngine profile;


    std::cout
    << "\n===============================\n"
    << "     HARDWARE INFORMATION\n"
    << "===============================\n\n";


    std::cout
    << "Architecture....."
    << hardware.getArchitecture()
    << "\n\n";


    std::string rawCPU = hardware.getCPU();

    std::cout
    << "CPU.............."
    << CPUParser::cleanName(rawCPU)
    << "\n";

    std::cout
    << "CORES............"
    << CPUParser::cores()
    << "\n";

    std::cout
    << "THREADS.........."
    << CPUParser::threads()
    << "\n";

    std::cout
    << "CLOCK............"
    << CPUParser::frequency(rawCPU)
    << "\n";

    std::cout
    << "CPU CLASS........"
    << CPUParser::cpuClass(rawCPU)
    << "\n";


    std::cout
    << "RAM.............."
    << hardware.getRAM()
    << "\n";


    std::string rawGPU = hardware.getGPU();

    std::cout
    << "GPU.............."
    << GPUParser::cleanName(rawGPU)
    << "\n";

    std::cout
    << "GPU TYPE........."
    << GPUParser::type(rawGPU)
    << "\n";


    auto rating = profile.analyze(
        hardware.getCPU(),
                                  hardware.getGPU(),
                                  hardware.getRAMGB()
    );


    std::cout
    << "CIOS SCORE........"
    << rating.total
    << "/100\n";


    std::cout
    << "CLASSIFICATION...."
    << rating.tier
    << "\n";
}
