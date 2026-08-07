#pragma once

#include "cios_features/core_state.hpp"

#include <iosfwd>
#include <string_view>

namespace cios::features {

// Returns true when the input was handled by this feature pack.
bool handleFeatureCommand(
    std::string_view rawInput,
    CoreStateManager& stateManager,
    std::istream& in,
    std::ostream& out
);

void printRoutineCatalog(std::ostream& out);
void runAssessment(std::istream& in, std::ostream& out);

}  // namespace cios::features
