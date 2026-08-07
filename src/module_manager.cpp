#include "module_manager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>


namespace fs = std::filesystem;


void ModuleManager::scan()
{
    modules.clear();

    std::string path =
        "../rootfs/opt/cios/modules";


    if(!fs::exists(path))
        return;


    for(auto& folder : fs::directory_iterator(path))
    {
        if(!folder.is_directory())
            continue;


        ModuleInfo module;

	std::string config =
    folder.path().string()
    + "/module.conf";


	if(!fs::exists(config))
	{
    		continue;
	}


	std::ifstream file(config);

        std::string line;


        while(std::getline(file,line))
        {
            auto pos=line.find("=");

            if(pos == std::string::npos)
                continue;


            auto key=line.substr(0,pos);
            auto value=line.substr(pos+1);


            if(key=="NAME")
                module.name=value;

            if(key=="VERSION")
                module.version=value;

            if(key=="TYPE")
                module.type=value;

            if(key=="STATUS")
                module.status=value;
        }


        modules.push_back(module);
    }
}



void ModuleManager::display()
{
    std::cout
    << "\n===============================\n"
    << "       CIOS MODULE MANAGER\n"
    << "===============================\n\n";


    for(auto& m : modules)
    {
        std::cout
        << "[" << m.status << "] "
        << m.name << "\n"
        << " Version: "
        << m.version << "\n"
        << " Type: "
        << m.type << "\n\n";
    }
}
