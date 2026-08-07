#include "cios.hpp"

#include "boot.hpp"
#include "command_registry.hpp"
#include "terminal.hpp"

#include <filesystem>
#include <iostream>
#include <string>

namespace
{
    std::string findConfigPath()
    {
        const std::string candidates[] = {
            "/opt/cios/config/cios.json",
            "/etc/cios/cios.json",
            "../config/cios.json",
            "config/cios.json"
        };

        for (const std::string& path : candidates)
        {
            if (std::filesystem::exists(path))
            {
                return path;
            }
        }

        return {};
    }
}

void CIOS::start()
{
    const std::string configPath =
    findConfigPath();

    if (
        !configPath.empty() &&
        config.load(configPath)
    )
    {
        std::cout
        << "System: "
        << config.systemName
        << '\n';

        std::cout
        << "Base Intelligence: "
        << config.intelligence
        << '\n';

        std::cout
        << "Personality: "
        << config.personality
        << "\n\n";
    }
    else
    {
        std::cout
        << "Configuration failed.\n";
    }

    bootSequence();

    CommandManager manager;
    registerCommands(manager);

    terminalLoop(manager);
}
