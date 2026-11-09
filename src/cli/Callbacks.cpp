#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "core/Manager.hpp"

void addCourseCallback(Manager& manager, const std::string& code)
{
  auto result = manager.addCourse(code);
  if (!result)
    handleAddCourseError(result, code);
  else
    std::cout << "Successfully added course " << code << std::endl;
}

void removeCourseCallback(Manager& manager, const std::string& code)
{
  auto result = manager.removeCourse(code);
  if (!result)
    handleRemoveCourseError(result, code);
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
  auto result = manager.addAssessment(code, name, weight, grade);
  if (!result)
    handleAddAssessmentError(result, code, name);
  else
    std::cout << "Successfully added assessment " << name << " to " << code << std::endl;
}

void removeAssessmentCallback(Manager& manager, const std::string& code, const std::string& name)
{
  auto result = manager.removeAssessment(code, name);
  if (!result)
    handleRemoveAssessmentError(result, code, name);
  else
    std::cout << "Successfully removed assessment " << name << " to " << code << std::endl;
}

void updateGradeCallback(Manager& manager, const std::string& code, const std::string& name, std::string grade)
{
  auto result = manager.updateGrade(code, name, grade);
  if (!result)
  {
    handleUpdateGradeError(result, code, name);
    return;
  }

  if (grade.find("/") == std::string::npos and
      grade.find("%") == std::string::npos) // If the input was given in percent without "%"
    grade += "%";
  if (*result == "new") // first time a grade is being set
    std::cout << "Added grade " << grade << " to assessment " << name << " in " << code << std::endl;
  else
    std::cout << "Grade for " << name << " in " << code << " updated from " << *result << " to " << grade << std::endl;
}

void showGradeCallback(Manager& manager, bool showScore, bool showPercent, bool showExact, const std::string& code, const std::string& name)
{
  if (name == "") // If no assessment name was given, show the grade in the given course
  {
    auto result = manager.getCurrentGrade(code);
    if (!result)
    {
      handleShowCourseGradeError(result, code);
      return;
    }

    if (not showExact)
      *result = round(*result);

    std::cout << "Current grade in " << code << " is " << *result << "%" << std::endl;
  }
  else
  {
    if (showScore)
    {
      auto result = manager.getScore(code, name);
      if (!result)
      {
        handleShowAssessmentGradeError(result, code, name);
        return;
      }

      if (*result != "")
        std::cout << "Grade for " << name << " in " << code << " is " << *result << std::endl;
      else
        std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
    }
    else if (showPercent)
    {
      auto result = manager.getPercentage(code, name);
      if (!result)
      {
        handleShowAssessmentGradeError(result, code, name);
        return;
      }
      if (*result >= 0)
        std::cout << "Grade for " << name << " in " << code << " is " << *result << "%" << std::endl;
      else
        std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
    }
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
