#include <CLI/App.hpp>
#include <iostream>

#include "cli/Commands.hpp"
#include "core/Manager.hpp"

int main(int argc, char** argv)
{
  CLI::App app{ "A tracker for grades" };
  Manager manager;

  bool loaded = true;
  if (!manager.load()) // TODO manage an error here
  {
    loaded = false;
    std::cout << "failed to load" << std::endl;
  }
  argv = app.ensure_utf8(argv);

  registerCommands(app, manager);

  CLI11_PARSE(app, argc, argv);

  auto saveStatus = Result<void>();
  if (loaded) // Don't overwrite the save if it wasn't loaded properly
    saveStatus = manager.save();

  if (!saveStatus)
    std::cout << "Could not save changes to file" << std::endl;

  return 0;
}
