#include "arch_manager.hpp"

#include <cstdio>
#include <array>


std::string runCommand(const char* command)
{
    std::array<char,256> buffer;
    std::string result;

    FILE* pipe = popen(command,"r");

    if(!pipe)
        return "UNKNOWN";


    while(fgets(buffer.data(),buffer.size(),pipe))
    {
        result += buffer.data();
    }

    pclose(pipe);

    if(!result.empty() && result.back()=='\n')
        result.pop_back();

    return result;
}



std::string ArchManager::getOS()
{
    return runCommand(
        "grep PRETTY_NAME /etc/os-release | cut -d= -f2"
    );
}


std::string ArchManager::getKernel()
{
    return runCommand(
        "uname -r"
    );
}


std::string ArchManager::getPackageManager()
{
    return "pacman";
}


std::string ArchManager::getInitSystem()
{
    return "systemd";
}


std::string ArchManager::getDesktop()
{
    return runCommand(
        "echo $XDG_CURRENT_DESKTOP"
    );
}
