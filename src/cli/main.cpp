#include <CLI/App.hpp>
#include <iostream>

#include "cli/Commands.hpp"
#include "core/Manager.hpp"

int main(int argc, char** argv)
{
  CLI::App app{ "A tracker for grades" };
  Manager manager;
  argv = app.ensure_utf8(argv);

  registerCommands(app, manager);

  CLI11_PARSE(app, argc, argv);

  auto saveStatus = manager.save();

  if (!saveStatus)
    std::cout << "Could not save changes to file" << std::endl;

  return 0;
}
