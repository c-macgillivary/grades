#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "CLI/CLI.hpp"
#include "core/Manager.hpp"

void registerCommands(CLI::App& app, Manager& manager);

#endif // !COMMANDS_HPP
