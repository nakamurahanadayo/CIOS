#include "module_loader.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>


namespace fs = std::filesystem;


void ModuleLoader::scan()
{
    std::cout << "\nMODULE SCAN\n\n";


    std::string path = "../rootfs/opt/cios/modules";


    if(!fs::exists(path))
    {
        std::cout
        << "No module directory found.\n";

        return;
    }


    for(auto& entry : fs::directory_iterator(path))
    {
        if(entry.is_directory())
        {
            std::string name =
                entry.path().filename().string();


                std::cout
                << name;

                for(size_t i = name.length(); i < 22; i++)
                {
                    std::cout << ".";
                }


            std::string conf =
                entry.path().string()
                + "/module.conf";


            if(fs::exists(conf))
            {
                std::ifstream file(conf);

                std::string line;

                while(std::getline(file,line))
                {
                    if(line.find("STATUS=")==0)
                    {
                        std::cout
                        << line.substr(7);

                        break;
                    }
                }
            }
            else
            {
                std::cout
                << "NO CONFIG";
            }


            std::cout << "\n";
        }
    }
}
