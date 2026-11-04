#include <CLI/App.hpp>

#include "ClassManager.hpp"

int main(int argc, char** argv)
{
  CLI::App app{ "A tracker for grades" };
  ClassManager manager;
  argv = app.ensure_utf8(argv);

  auto class_cmd = app.add_subcommand("class", "Manage classes");
  auto class_add = class_cmd->add_subcommand("add", "Add classes");
  std::string className = "";
  class_add->add_option("name", className, "Name of the class to be added")->required();
  class_add->callback([&]()
                      { manager.addClass(className); });

  CLI11_PARSE(app, argc, argv);

  return 0;
}
