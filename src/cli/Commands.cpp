#include <iostream>

#include "cli/Callbacks.hpp"
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
                       { addCourseCallback(manager, code); });

  // course remove subcommand
  auto course_remove = course_cmd->add_subcommand("remove", "Remove courses");
  course_remove->alias("r");
  course_remove->add_option("<course_code>", code, "Code of the course to be removed")->required();
  course_remove->callback([&]()
                          { removeCourseCallback(manager, code); });

  // course list subcommand
  auto course_list = course_cmd->add_subcommand("list", "List courses");
  course_list->alias("l");
  course_list->callback([&]()
                        { listCoursesCallback(manager); });
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
                            { addAssessmentCallback(manager, code, name, weight, grade); });

  // assessment remove subcommand
  auto assessments_remove = assessments_cmd->add_subcommand("remove", "Remove assessments");
  assessments_remove->alias("r");
  assessments_remove->add_option("<course_code>", code, "Code of the course with the assessment")->required();
  assessments_remove->add_option("<assessment_name>", name, "Name of the assessment to be removed")->required();
  assessments_remove->callback([&]()
                               { removeAssessmentCallback(manager, code, name); });
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
                         { updateGradeCallback(manager, code, name, grade); });

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
                       { showGradeCallback(manager, showScore, showPercent, showExact, code, name); });
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
                 { dumpCallback(manager); });

  auto clean = app.add_subcommand("clean", "WARNING !!! REMOVES ALL STORED DATA !!! WARNING");
  clean->require_subcommand(0);
  clean->callback([&]()
                  { cleanCallback(manager); });
}
