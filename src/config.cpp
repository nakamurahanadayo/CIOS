#include "config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


bool Config::load(const std::string& path)
{
    std::ifstream file(path);

    if(!file.is_open())
    {
        return false;
    }


    json data;
    file >> data;


    systemName = data["system_name"];
    version = data["version"];
    intelligence = data["base_intelligence"];
    personality = data["personality"];


    return true;
}
