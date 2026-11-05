#ifndef COMMANDS_H
#define COMMANDS_H

#include "CLI/CLI.hpp"
#include "core/Manager.hpp"

void registerCommands(CLI::App& app, Manager& manager);

#endif // !COMMANDS_H
