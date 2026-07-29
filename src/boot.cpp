#include "boot.hpp"

#include <iostream>
#include <thread>
#include <chrono>

void bootSequence()
{
    std::cout << "====================================\n";
    std::cout << "CIOS v0.1\n";
    std::cout << "Central Intelligence Operating System\n";
    std::cout << "====================================\n\n";

    std::cout << "Initializing system...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Loading Caroline.............OK\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Reasoning Engine.............OFFLINE\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Personality Module...........WAITING\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\nCIOS READY.\n\n";
}

