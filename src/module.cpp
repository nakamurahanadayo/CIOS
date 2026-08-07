#include "module.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void ModuleManager::scan(const std::string& path)
{
    modules.clear();

    for(auto& folder : std::filesystem::directory_iterator(path))
    {
        std::string file = folder.path().string() + "/module.json";

        if(std::filesystem::exists(file))
        {
            std::ifstream input(file);
            json data;
            input >> data;

            Module m;
            m.name = data["name"];
            m.status = data["status"];
            m.provider = data["provider"];

            modules.push_back(m);
        }
    }
}

void ModuleManager::showModules()
{
    std::cout << "\nMODULE STATUS\n\n";

    for(auto& m : modules)
    {
        std::cout << m.name
                  << "........"
                  << m.status
                  << " (" << m.provider << ")\n";
    }

    std::cout << "\n";
}
