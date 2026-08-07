#include "command_registry.hpp"
#include "command_manager.hpp"
#include "help.hpp"
#include "status.hpp"
#include "model.hpp"
#include "modules.hpp"
#include "hardware.hpp"
#include "../commands/caroline.hpp"
#include "../commands/wheatley.hpp"
#include "../commands/aegis.hpp"
#include <memory>
#include "../commands/start.hpp"
#include "../commands/shell.hpp"

void registerCommands(CommandManager& manager)
{
    manager.registerCommand(std::make_unique<StatusCommand>());
    manager.registerCommand(std::make_unique<ModelCommand>());
    manager.registerCommand(std::make_unique<ModulesCommand>());
    manager.registerCommand(std::make_unique<HardwareCommand>());
    manager.registerCommand(std::make_unique<HelpCommand>());
    manager.registerCommand(std::make_unique<CarolineCommand>());
    manager.registerCommand(std::make_unique<WheatleyCommand>());
    manager.registerCommand(std::make_unique<AegisCommand>());
    manager.registerCommand(std::make_unique<StartCommand>());
    manager.registerCommand(std::make_unique<ShellCommand>());
}
