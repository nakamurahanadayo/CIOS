#include "caroline.hpp"

#include "../ai/llm_client.hpp"
#include "cios_features/core_state.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace
{
    std::string activePersonality()
    {
        cios::features::CoreStateManager featureState(
            "/tmp/cios-cores.json"
        );

        featureState.load();

        const auto& state = featureState.state();

        if (state.mainBodyController == "wheatley") {
            return "Wheatley";
        }

        if (state.gladosCore) {
            return "GLaDOS";
        }

        if (state.carolineCore) {
            return "Caroline";
        }

        return "UNKNOWN";
    }
}

void Caroline::initialize(
    std::string modelPath,
    std::string personalityName
)
{
    model = std::move(modelPath);

    (void)personalityName;

    personality = activePersonality();

    std::cout
        << personality
        << " initializing...\n";

    std::cout
        << "Model: "
        << model
        << "\n";

    std::cout
        << "Personality: "
        << personality
        << "\n";
}

void Caroline::status()
{
    personality = activePersonality();

    std::cout
        << "\n"
        << personality
        << " STATUS\n";

    std::cout
        << "Engine.........llama.cpp\n";

    std::cout
        << "Model.........."
        << model
        << "\n";

    std::cout
        << "Personality...."
        << personality
        << "\n";
}

std::string Caroline::respond(
    std::string input
)
{
    personality = activePersonality();

    LLMClient client;

    return client.send(input);
}
