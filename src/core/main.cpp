#include <CLI/App.hpp>
#include <iostream>

#include "cli/ClassCommands.hpp"
#include "core/ClassManager.hpp"

int main(int argc, char** argv)
{
  CLI::App app{ "A tracker for grades" };
  ClassManager manager;
  argv = app.ensure_utf8(argv);

  registerCommands(app, manager);

  CLI11_PARSE(app, argc, argv);

  manager.save();

  return 0;
}
