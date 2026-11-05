#include <CLI/App.hpp>
#include <iostream>

#include "core/ClassManager.hpp"

int main(int argc, char** argv)
{
  CLI::App app{ "A tracker for grades" };
  ClassManager manager;
  argv = app.ensure_utf8(argv);

  auto class_cmd = app.add_subcommand("class", "Manage classes");
  auto class_add = class_cmd->add_subcommand("add", "Add classes");
  auto class_remove = class_cmd->add_subcommand("remove", "Remove classes");
  auto class_list = class_cmd->add_subcommand("list", "List classes");

  std::string code = "";
  class_add->add_option("<course_code>", code, "Code of the class to be added")->required();
  class_add->callback([&]()
                      { manager.addClass(code); });

  class_remove->add_option("<course_code>", code, "Code of the class to be removed")->required();
  class_remove->callback([&]()
                         { manager.removeClass(code); });

  class_list->callback([&]()
                       { 
                         auto classes = manager.getClasses();
                         for (auto& [code, c] : classes)
                           std::cout << code << std::endl; });

  CLI11_PARSE(app, argc, argv);

  manager.save();

  return 0;
}
