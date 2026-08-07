#include "start.hpp"

#include "../src/ai/ai_manager.hpp"

void StartCommand::execute()
{
    AIManager::engine.start();
}
