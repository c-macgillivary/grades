#include <iostream>

#include "cli/Commands.hpp"

void registerClassCommands(CLI::App& app, Manager& manager)
{
  auto class_cmd = app.add_subcommand("class", "Manage classes");
  class_cmd->alias("c");
  class_cmd->require_subcommand(1);

  auto class_add = class_cmd->add_subcommand("add", "Add classes");
  class_add->alias("a");

  auto class_remove = class_cmd->add_subcommand("remove", "Remove classes");
  class_remove->alias("r");

  auto class_list = class_cmd->add_subcommand("list", "List classes");
  class_list->alias("l");

  static std::string code = "";

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
}

void registerAssessmentCommands(CLI::App& app, Manager& manager)
{
  auto assessments_cmd = app.add_subcommand("assessments", "Manage assessments");
  assessments_cmd->alias("a");
  assessments_cmd->require_subcommand(1);

  auto assessments_add = assessments_cmd->add_subcommand("add", "Add assessments");
  assessments_add->alias("a");

  static std::string code = "";
  static std::string name = "";
  static double weight = 0.0;
  static double grade = -1.0;

  assessments_add->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_add->add_option("<assessment_name>", name, "Name of the assessment to be added")->required();
  assessments_add->add_option("<weight>", weight, "Weight of the assessment towards the final grade")->required();
  assessments_add->add_option("[grade]", grade, "Grade achieved");

  assessments_add->callback([&]()
                      { manager.addAssessment(code, name, weight, grade); });
}

void registerCommands(CLI::App& app, Manager& manager)
{
  registerClassCommands(app, manager);
  registerAssessmentCommands(app, manager);
}
