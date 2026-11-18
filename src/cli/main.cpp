#include <CLI/App.hpp>

#include "cli/Callbacks.hpp"
#include "cli/Commands.hpp"
#include "cli/Persistance.hpp"
#include "core/Manager.hpp"

int main(int argc, char** argv)
{
  Manager manager;

  // Bypass parsing for cleaning stale data files
  if (argc == 2 and std::string_view(argv[1]) == "clean")
  {
    cleanCallback(manager);
    return 0;
  }

  CLI::App app{ "A tracker for grades" };

  if (not load(manager))
    return 1;

  argv = app.ensure_utf8(argv);

  registerCommands(app, manager);

  CLI11_PARSE(app, argc, argv);

  save(manager);

  return 0;
}
