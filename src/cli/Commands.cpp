#include <iostream>

#include "cli/Commands.hpp"

void registerCourseCommands(CLI::App& app, Manager& manager)
{
  // course subcommand
  auto course_cmd = app.add_subcommand("course", "Manage courses");
  course_cmd->alias("c");
  course_cmd->require_subcommand(1);

  // course add subcommand
  auto course_add = course_cmd->add_subcommand("add", "Add courses");
  course_add->alias("a");
  static std::string code = "";
  course_add->add_option("<course_code>", code, "Code of the course to be added")->required();
  course_add->callback([&]()
                       {
                        if (manager.addCourse(code) == 0)
                          std::cout << "Successfully added course " << code << std::endl; });

  // course remove subcommand
  auto course_remove = course_cmd->add_subcommand("remove", "Remove courses");
  course_remove->alias("r");
  course_remove->add_option("<course_code>", code, "Code of the course to be removed")->required();
  course_remove->callback([&]()
                          {
                          if (manager.removeCourse(code) == 0)
                            std::cout << "Successfully removed course " << code << std::endl; });

  // course list subcommand
  auto course_list = course_cmd->add_subcommand("list", "List courses");
  course_list->alias("l");
  course_list->callback([&]()
                        {
                         auto courses = manager.getCourses();
                         for (auto& [code, c] : courses)
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
  static std::string weight = "";
  static std::string grade = "";

  assessments_add->alias("a");
  assessments_add->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_add->add_option("<assessment_name>", name, "Name of the assessment to be added")->required();
  assessments_add->add_option("<weight>", weight, "Weight of the assessment towards the final grade\nInput either as a percentage of final grade (83.7) or points (25.5/30)")->required();
  assessments_add->add_option("grade", grade, "Grade achieved, either as a percentage (83.7) or points (25.5/30)");
  assessments_add->callback([&]()
                            {
                              if (manager.addAssessment(code, name, weight, grade) == 0)
                                std::cout << "Successfully added assessment " << name << " to " << code << std::endl; });

  // assessment remove subcommand
  auto assessments_remove = assessments_cmd->add_subcommand("remove", "Remove assessments");
  assessments_remove->alias("r");
  assessments_remove->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_remove->add_option("<assessment_name>", name, "Name of the assessment to be removed")->required();
  assessments_remove->callback([&]()
                               {
                                if (manager.removeAssessment(code, name) == 0)
                                  std::cout << "Successfully removed assessment " << name << " from " << code << std::endl; });
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
                          if (ret == "new") // first time a grade is being set
                            std::cout << "Added grade " << grade << " to assessment " << name << " in " << code << std::endl;
                          else if (ret != "") // update grade did not error
                            std::cout << "Grade for " << name << " in " << code <<
                                         " updated from " << ret << " to " << grade << std::endl; });

  // grade show subcommand
  auto grade_show = grade_cmd->add_subcommand("show", "Show course/assessment grades");
  static bool showScore = false;
  static bool showPercent = true; // default true
  static bool showExact = false;

  grade_show->alias("s");
  auto scoreFlag = grade_show->add_flag("-s,--score", showScore, "Show assessment grade as a score out of X points");
  grade_show->add_flag("-p,--percent,--percentage", showPercent, "Show grade as a percentage (default)")->excludes("-s");
  scoreFlag->excludes("-p");
  grade_show->get_formatter()->column_width(35);
  grade_show->add_option("<course_code>", code, "The course the grade is in")->required();
  grade_show->add_option("assessment_name", name, "The assessment's grade to be shown. Leave blank to show course grade");
  grade_show->add_flag("-e,--exact", showExact, "Show exact grade (no rounding)")->excludes("assessment_name", "-s");
  grade_show->callback([&]()
                       {
                        if (name != "")
                        {
                          if (showScore)
                          {
                            std::string score = manager.getScore(code, name);
                            if (score != "") // getScore() did not error
                              std::cout << "Grade for " << name << " in " << code << " is " << score << std::endl;
                            else // TODO add error checking for assignment/course does not exist
                              std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
                              }
                          else
                          {
                            double percent = manager.getPercentage(code, name);
                            if (percent >= 0) // update grade did not error
                              std::cout << "Grade for " << name << " in " << code << " is " << percent << "%" << std::endl;
                            else // TODO add error checking for assignment/course does not exist
                              std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
                          }
                        }
                        else
                        {
                          double grade = manager.getCurrentGrade(code);
                          if (not showExact)
                            grade = round(grade);
                          if (grade >= 0)
                            std::cout << "Current grade in " << code << " is " << grade << "%" << std::endl;
                        } });
}

void registerCommands(CLI::App& app, Manager& manager)
{
  registerCourseCommands(app, manager);
  registerAssessmentCommands(app, manager);
  registerGradeCommands(app, manager);

  auto dump = app.add_subcommand("dump", "DEBUG: Dump all saved data to the screen");
  dump->alias("d");
  dump->require_subcommand(0);

  dump->callback([&]()
                 { std::cout << manager.dump() << '\n'; });

  auto clean = app.add_subcommand("clean", "WARNING !!! REMOVES ALL STORED DATA !!! WARNING");
  clean->require_subcommand(0);
  clean->callback([&]()
                  {
                    std::cout << "This will delete ALL stored data. Are you sure? Type \"clean\" to confirm: ";
                    std::string input = "";
                    std::getline(std::cin, input);
                    if (input == "clean")
                    {
                      manager.clean();
                      std::cout << "Successfully deleted ALL stored data at " << manager.getPath() << std::endl;
                    } });
}
