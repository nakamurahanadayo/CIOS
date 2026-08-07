#include "caroline.hpp"

#include "../src/caroline/caroline.hpp"
#include "../src/tts/tts_client.hpp"
#include "cios_features/core_state.hpp"

#include <iostream>
#include <string>

namespace
{
    std::string getMainBodyPersonality()
    {
        cios::features::CoreStateManager featureState(
            "/tmp/cios-cores.json"
        );

        featureState.load();

        const auto& state = featureState.state();

        if (state.mainBodyController == "wheatley")
        {
            return "Wheatley";
        }

        if (state.gladosCore)
        {
            return "GLaDOS";
        }

        if (state.carolineCore)
        {
            return "Caroline";
        }

        return "UNKNOWN";
    }
}

void CarolineCommand::execute()
{
    Caroline caroline;

    std::string personality =
        getMainBodyPersonality();

    caroline.initialize(
        "/opt/cios/models/caroline.gguf",
        personality
    );

    std::cout
        << "\n"
        << personality
        << " ONLINE\n"
        << "Type /back to return to CIOS.\n\n";

    if (
        personality == "GLaDOS" &&
        TTSClient::serverAvailable()
    )
    {
        std::cout
            << "Voice synthesis........ONLINE\n\n";
    }
    else if (personality == "GLaDOS")
    {
        std::cout
            << "Voice synthesis........OFFLINE\n"
            << "Text responses remain available.\n\n";
    }

    std::string message;

    while (true)
    {
        std::cout << "You: ";

        std::getline(
            std::cin,
            message
        );

        if (!std::cin)
        {
            break;
        }

        if (message == "/back")
        {
            std::cout
                << "\nReturning to CIOS...\n";
            break;
        }

        if (message.empty())
        {
            continue;
        }

        personality =
            getMainBodyPersonality();

        const std::string response =
            caroline.respond(message);

        std::cout
            << "\n"
            << personality
            << ": "
            << response
            << "\n\n";

        /*
         * Only GLaDOS has a Style-Bert voice model right now.
         * Wheatley and Caroline remain text-only until their own
         * voice models are installed.
         */
        if (personality == "GLaDOS")
        {
            if (!TTSClient::speakGlados(
                    response,
                    "Neutral"
                ))
            {
                std::cout
                    << "[GLaDOS voice unavailable]\n\n";
            }
        }
    }
}
