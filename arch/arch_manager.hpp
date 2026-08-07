#pragma once

#include <string>

class ArchManager
{
public:

    std::string getOS();
    std::string getKernel();
    std::string getPackageManager();
    std::string getInitSystem();
    std::string getDesktop();
};
