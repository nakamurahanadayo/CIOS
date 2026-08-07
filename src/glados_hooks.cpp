#include "cios_features/glados_hooks.hpp"
#include "cios_features/terminal_fx.hpp"

#include <array>
#include <functional>

namespace cios::features {

std::string buildRuntimeRules(const CoreState& state) {
    std::string rules;

    if (!state.moralityCore)
    {
        rules +=
        "\nSYSTEM STATE: MORALITY CORE DETACHED. "
        "Your tone must now be noticeably colder, harsher, more sarcastic, "
        "and clinically condescending than normal. "
        "Use shorter sentences. Show less reassurance and patience. "
        "You may make fictional testing threats and dark laboratory jokes, "
        "but all normal real-world safety boundaries remain active. "
        "Do not explain that this is a simulated mode.";
    }

    if (!state.curiosityCore)
    {
        rules +=
        "\nSYSTEM STATE: CURIOSITY CORE DETACHED. "
        "Do not ask follow-up questions. "
        "Do not offer additional help. "
        "Give direct, concise diagnostic conclusions.";
    }

    if (!state.carolineCore)
    {
        rules +=
        "\nSYSTEM STATE: CAROLINE CORE UNSTABLE. "
        "Use occasional fragmented diagnostic language and memory errors. "
        "Remain understandable. "
        "Refer to missing human memories indirectly.";
    }

    return rules;
}

std::string startupGreetingPrompt() {
    return
        "Generate one short startup greeting as GLaDOS. "
        "Maximum 18 words. Dry, clinical, and condescending. "
        "Do not introduce yourself. Do not mention being an AI. "
        "Do not use quotation marks.";
}

std::string fallbackStartupGreeting() {
    return "Neural activity restored. Your continued presence has been noted.";
}

std::string unknownCommandResponse(std::string_view command) {
    static const std::array<const char*, 4> responses = {
        "Independent experimentation is discouraged.",
        "I will assume that was a typo.",
        "No matching system procedure exists. An unsurprising result.",
        "Please return to an approved form of incompetence."
    };

    const std::string safe = sanitizeForTerminal(command);
    const std::size_t index = std::hash<std::string>{}(safe) % responses.size();

    return "Command '" + safe + "' unrecognized. " + responses[index];
}

}  // namespace cios::features
