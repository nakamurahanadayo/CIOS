#include "aegis.hpp"

#include "../src/ai/llm_client.hpp"
#include "cios_features/core_state.hpp"

#include <iostream>
#include <string>

namespace
{
    constexpr const char* statePath =
        "/tmp/cios-cores.json";

    void setActivePersonality(const std::string& personality)
    {
        cios::features::CoreStateManager stateManager(
            statePath
        );

        stateManager.load();
        stateManager.state().activePersonality = personality;
        stateManager.save();
    }
}

void AegisCommand::execute()
{
    setActivePersonality("aegis");

    std::cout
        << "AEGIS SECURITY NETWORK INITIALIZING...\n"
        << "Model: unknown\n"
        << "Personality: AEGIS\n\n"
        << "FACILITY SENSOR GRID.................ONLINE\n"
        << "INTRUSION ANALYSIS...................ACTIVE\n"
        << "THREAT DATABASE......................LOADED\n\n"
        << "AEGIS ONLINE\n"
        << "Type /back to return to CIOS.\n\n";

    LLMClient client;
    std::string message;

    while (true)
    {
        std::cout << "You: ";
        std::getline(std::cin, message);

        if (!std::cin)
        {
            setActivePersonality("central");
            break;
        }

        if (message == "/back")
        {
            setActivePersonality("central");
            std::cout << "\nReturning to CIOS...\n";
            break;
        }

        if (message.empty()) {
            continue;
        }

        std::cout
            << "\nAEGIS: "
            << client.send(message)
            << "\n\n";
    }
}
