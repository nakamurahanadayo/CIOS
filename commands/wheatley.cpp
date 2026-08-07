#include "wheatley.hpp"

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

void WheatleyCommand::execute()
{
    setActivePersonality("wheatley");

    std::cout
        << "Wheatley initializing...\n"
        << "Model: unknown\n"
        << "Personality: Wheatley\n\n"
        << "Wheatley ONLINE\n"
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
            << "\nWheatley: "
            << client.send(message)
            << "\n\n";
    }
}
