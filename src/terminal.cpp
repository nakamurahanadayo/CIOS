#include "terminal.hpp"
#include "cios_features/core_state.hpp"
#include "cios_features/lore_commands.hpp"
#include "cios_features/glados_hooks.hpp"
#include "../src/ai/ai_manager.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

void terminalLoop(CommandManager& manager)
{

    static cios::features::CoreStateManager featureState(
        "/tmp/cios-cores.json"
    );
    static bool featureStateLoaded = featureState.load();
    (void)featureStateLoaded;

    std::string input;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (!std::cin)
        {
            AIManager::engine.stop();
            break;
        }

        if (input == "/exit")
        {
            std::cout << "\nStopping Intelligence Engine...\n";
            AIManager::engine.stop();

            std::cout << "Shutting down CIOS...\n";
            std::cout.flush();

            const int result =
            std::system("systemctl poweroff");

            if (result != 0)
            {
                std::cout
                << "Poweroff failed. Returning to terminal.\n";
            }

            break;
        }
        if (cios::features::handleFeatureCommand(
            input,
            featureState,
            std::cin,
            std::cout))
        {
            continue;
        }

        manager.execute(input);
    }
}
