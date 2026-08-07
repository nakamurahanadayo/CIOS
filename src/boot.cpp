#include "boot.hpp"

#include "module_loader.hpp"

#include <chrono>
#include <iostream>
#include <thread>

void bootSequence()
{
    using namespace std::chrono_literals;

    std::cout << "====================================\n";
    std::cout << "CIOS v0.1\n";
    std::cout << "Central Intelligence Operating System\n";
    std::cout << "====================================\n\n";

    std::cout << "Initializing system...\n";
    std::this_thread::sleep_for(500ms);

    std::cout << "Loading Caroline Core........OK\n";
    std::this_thread::sleep_for(500ms);

    std::cout << "Loading GLaDOS Personality...OK\n";
    std::this_thread::sleep_for(500ms);

    std::cout << "Reasoning Engine..............OFFLINE\n";
    std::this_thread::sleep_for(500ms);

    std::cout << "Personality Module............WAITING\n";
    std::this_thread::sleep_for(500ms);

    ModuleLoader loader;
    loader.scan();

    std::cout << "\nCIOS READY.\n\n";
}
