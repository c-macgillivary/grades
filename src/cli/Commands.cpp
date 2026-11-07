#include <iostream>

#include "cli/Commands.hpp"

void registerClassCommands(CLI::App& app, Manager& manager)
{
  // class subcommand
  auto class_cmd = app.add_subcommand("class", "Manage classes");
  class_cmd->alias("c");
  class_cmd->require_subcommand(1);

  // class add subcommand
  auto class_add = class_cmd->add_subcommand("add", "Add classes");
  class_add->alias("a");
  static std::string code = "";
  class_add->add_option("<course_code>", code, "Code of the class to be added")->required();
  class_add->callback([&]()
                      { manager.addClass(code); });

  // class remove subcommand
  auto class_remove = class_cmd->add_subcommand("remove", "Remove classes");
  class_remove->alias("r");
  class_remove->add_option("<course_code>", code, "Code of the class to be removed")->required();
  class_remove->callback([&]()
                         { manager.removeClass(code); });

  // class list subcommand
  auto class_list = class_cmd->add_subcommand("list", "List classes");
  class_list->alias("l");
  class_list->callback([&]()
                       {
                         auto classes = manager.getClasses();
                         for (auto& [code, c] : classes)
                           std::cout << code << std::endl; });
}

void registerAssessmentCommands(CLI::App& app, Manager& manager)
{
  // assessment subcommand
  auto assessments_cmd = app.add_subcommand("assessments", "Manage assessments");
  assessments_cmd->alias("a");
  assessments_cmd->require_subcommand(1);

  // assessment add subcommand
  auto assessments_add = assessments_cmd->add_subcommand("add", "Add assessments");
  static std::string code = "";
  static std::string name = "";
  static double weight = 0.0;
  static std::string grade = "";

  assessments_add->alias("a");
  assessments_add->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_add->add_option("<assessment_name>", name, "Name of the assessment to be added")->required();
  assessments_add->add_option("<weight>", weight, "Weight of the assessment towards the final grade")->required();
  assessments_add->add_option("[grade]", grade, "Grade achieved, either as a percentage (83.7) or points (25.5/30)");
  assessments_add->callback([&]()
                            { manager.addAssessment(code, name, weight, grade); });

  // assessment remove subcommand
  auto assessments_remove = assessments_cmd->add_subcommand("remove", "Remove assessments");
  assessments_remove->alias("r");
  assessments_remove->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_remove->add_option("<assessment_name>", name, "Name of the assessment to be removed")->required();
  assessments_remove->callback([&]()
                               { manager.removeAssessment(code, name); });
}

void registerGradeCommands(CLI::App& app, Manager& manager)
{
  // grade subcommand
  auto grade_cmd = app.add_subcommand("grade", "Manage grades");
  grade_cmd->alias("g");
  grade_cmd->require_subcommand(1);

  // grade update subcommand
  auto grade_update = grade_cmd->add_subcommand("update", "Update grades");
  static std::string code = "";
  static std::string name = "";
  static std::string grade = "";

  grade_update->alias("u");
  grade_update->add_option("<course_code>", code, "The course which has the grade to be updated")->required();
  grade_update->add_option("<assessment_name>", name, "The assessment's grade to be updated")->required();
  grade_update->add_option("<new_grade>", grade, "The new grade on the assessment")->required();
  grade_update->callback([&]()
                         {
                          std::string ret = manager.updateGrade(code, name, grade);
                          if (grade.find("/") == std::string::npos and
                              grade.find("%") == std::string::npos) // If the input was given in percent without "%"
                            grade += "%";
                          if (ret != "") // update grade did not error
                            std::cout << "Grade for " << name << " in " << code <<
                                         " updated from " << ret << " to " << grade << std::endl; });

  // grade show subcommand
  auto grade_show = grade_cmd->add_subcommand("show", "Show grades");
  static bool showScore = false;
  static bool showPercent = true; // default true

  grade_show->alias("s");
  auto scoreFlag = grade_show->add_flag("-s,--score", showScore, "Show grade as a score out of X points");
  grade_show->add_flag("-p,--percent,--percentage", showPercent, "Show grade as a percentage")->excludes("-s");
  scoreFlag->excludes("-p");
  grade_show->add_option("<course_code>", code, "The course the grade is in")->required();
  grade_show->add_option("<assessment_name>", name, "The assessment's grade to be shown")->required();
  grade_show->callback([&]()
                       {
                        if (showScore)
                        {
                          std::string score = manager.getScore(code, name);
                          if (score != "") // getScore() did not error
                            std::cout << "Grade for " << name << " in " << code << " is " << score << std::endl;
                          else // TODO add error checking for assignment/class does not exist
                            std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
                            }
                        else
                        {
                          double percent = manager.getPercentage(code, name);
                          if (percent >= 0) // update grade did not error
                            std::cout << "Grade for " << name << " in " << code << " is " << percent << "%" << std::endl;
                          else // TODO add error checking for assignment/class does not exist
                            std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
                        } });
}

void registerCommands(CLI::App& app, Manager& manager)
{
  registerClassCommands(app, manager);
  registerAssessmentCommands(app, manager);
  registerGradeCommands(app, manager);
}
