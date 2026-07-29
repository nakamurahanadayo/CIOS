#include "cios.hpp"

#include "boot.hpp"
#include "terminal.hpp"

void CIOS::start()
{
    bootSequence();

    terminalLoop();
}
