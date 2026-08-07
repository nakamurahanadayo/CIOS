#include "module_manager.hpp"
#include "modules.hpp"

void ModulesCommand::execute()
{
    ModuleManager manager;

    manager.scan();

    manager.display();
}
