#pragma once

#include "cios_features/core_state.hpp"

#include <string>
#include <string_view>

namespace cios::features {

std::string buildRuntimeRules(const CoreState& state);
std::string startupGreetingPrompt();
std::string fallbackStartupGreeting();
std::string unknownCommandResponse(std::string_view command);

}  // namespace cios::features
