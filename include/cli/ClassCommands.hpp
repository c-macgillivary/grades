#ifndef CLASS_COMMANDS_H
#define CLASS_COMMANDS_H

#include "CLI/CLI.hpp"
#include "core/ClassManager.hpp"

void registerCommands(CLI::App& app, ClassManager& manager);

#endif // !CLASS_COMMANDS_H
