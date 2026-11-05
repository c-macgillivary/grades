#ifndef CLASS_COMMANDS_H
#define CLASS_COMMANDS_H

#include "CLI/CLI.hpp"
#include "core/Manager.hpp"

void registerCommands(CLI::App& app, Manager& manager);

#endif // !CLASS_COMMANDS_H
