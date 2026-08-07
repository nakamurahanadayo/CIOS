#include <iostream>
#include "cios.hpp"
#include "cios_features/core_state.hpp"
#include "cios_features/terminal_fx.hpp"
int main()
{
    cios::features::CoreStateManager featureState;
    featureState.load();

    std::cout << "\033[2J\033[H";

    cios::features::setPalette(
        std::cout,
        featureState.state().palette
    );

    cios::features::setBlinkingBlockCursor(std::cout);

    CIOS cios;
    cios.start();

    cios::features::resetTerminalStyle(std::cout);
    return 0;
}

