#include "../arch/arch_manager.hpp"

#include <iostream>


void showSystem()
{
    ArchManager arch;


    std::cout
    << "\n===============================\n"
    << "       SYSTEM INFORMATION\n"
    << "===============================\n\n";


    std::cout
    << "OS................."
    << arch.getOS()
    << "\n";


    std::cout
    << "Kernel............."
    << arch.getKernel()
    << "\n";


    std::cout
    << "Package Manager...."
    << arch.getPackageManager()
    << "\n";


    std::cout
    << "Init System........"
    << arch.getInitSystem()
    << "\n";


    std::cout
    << "Desktop............"
    << arch.getDesktop()
    << "\n";
}
