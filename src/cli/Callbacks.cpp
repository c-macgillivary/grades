#include <iostream>

#include "core/Manager.hpp"

void addCourseCallback(Manager& manager, const std::string& code)
{
  if (!manager.addCourse(code))
    std::cout << "Could not add course " << code << std::endl;
  else
    std::cout << "Successfully added course " << code << std::endl;
}

void removeCourseCallback(Manager& manager, const std::string& code)
{
  if (!manager.removeCourse(code))
    std::cout << "Could not remove course " << code << std::endl;
  else
    std::cout << "Successfully removed course " << code << std::endl;
}

void listCoursesCallback(Manager& manager)
{
  auto courses = manager.getCourses();
  for (auto& [code, c] : courses)
    std::cout << code << std::endl;
}

void addAssessmentCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& weight, const std::string& grade)
{
  if (!manager.addAssessment(code, name, weight, grade))
    std::cout << "Could not add assessment " << name << " to " << code << std::endl;
  else
    std::cout << "Successfully added assessment " << name << " to " << code << std::endl;
}

void removeAssessmentCallback(Manager& manager, const std::string& code, const std::string& name)
{
  if (!manager.removeAssessment(code, name))
    std::cout << "Could not remove assessment " << name << " to " << code << std::endl;
  else
    std::cout << "Successfully removed assessment " << name << " to " << code << std::endl;
}

void updateGradeCallback(Manager& manager, const std::string& code, const std::string& name, std::string grade)
{
  auto ret = manager.updateGrade(code, name, grade);
  if (!ret)
  {
    std::cout << "Could not update grade for assessment " << name << " in " << code << std::endl;
    return;
  }
  if (grade.find("/") == std::string::npos and
      grade.find("%") == std::string::npos) // If the input was given in percent without "%"
    grade += "%";
  if (*ret == "new") // first time a grade is being set
    std::cout << "Added grade " << grade << " to assessment " << name << " in " << code << std::endl;
  else
    std::cout << "Grade for " << name << " in " << code << " updated from " << *ret << " to " << grade << std::endl;
}

void showGradeCallback(Manager& manager, bool showScore, bool showPercent, bool showExact, const std::string& code, const std::string& name)
{
  if (name != "")
  {
    if (showScore)
    {
      auto score = manager.getScore(code, name);
      if (!score)
      {
        std::cout << "Could not show grade for assessment " << name << " in " << code << std::endl;
        return;
      }
      if (*score != "")
        std::cout << "Grade for " << name << " in " << code << " is " << *score << std::endl;
      else
        std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
    }
    else
    {
      auto percent = manager.getPercentage(code, name);
      if (!percent)
      {
        std::cout << "Could not show grade for assessment " << name << " in " << code << std::endl;
        return;
      }
      if (*percent >= 0)
        std::cout << "Grade for " << name << " in " << code << " is " << *percent << "%" << std::endl;
      else
        std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
    }
  }
  else
  {
    auto grade = manager.getCurrentGrade(code);
    if (!grade)
    {
      std::cout << "Could not show grade for " << code << std::endl;
      return;
    }
    if (not showExact)
      *grade = round(*grade);
    if (*grade >= 0)
      std::cout << "Current grade in " << code << " is " << *grade << "%" << std::endl;
  }
}

void dumpCallback(Manager& manager)
{
  std::cout << manager.dump() << '\n';
}

void cleanCallback(Manager& manager)
{
  std::cout << "This will delete ALL stored data. Are you sure? Type \"clean\" to confirm: ";
  std::string input = "";
  std::getline(std::cin, input);
  if (input == "clean")
  {
    manager.clean();
    std::cout << "Successfully deleted ALL stored data at " << manager.getPath() << std::endl;
  }
}
