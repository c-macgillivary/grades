#include <iostream>
#include <optional>
#include <string>
#include <utility>

#include "cli/ErrorHandling.hpp"
#include "core/Grade.hpp"
#include "core/Manager.hpp"

std::pair<double, double> parseInput(const std::string& input)
{
  size_t pos = input.find("/");

  // If a percentage was given
  if (pos == std::string::npos)
    return { std::stod(input), 100.0};
  else
    return { std::stod(input.substr(0,pos)), std::stod(input.substr(pos + 1)) };
}

std::string gradeToPercent(const Grade& grade, bool exact = false)
{
  double percent = grade.ratio() * 100;
  if (not exact)
    percent = round(percent);
  return std::format("{}", percent) + "%";
}

std::string gradeToScore(const Grade& grade)
{
  auto [part, total] = grade.pair();
  return std::format("{}/{}", part, total);
}

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
  if (courses.empty())
    std::cout << "There are no courses saved.\nAdd one with \"grades course add <course_name>\"" << std::endl;
  else
    for (auto& [code, c] : courses)
      std::cout << code << std::endl;
}

void addAssessmentCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& weightInput, const std::string& gradeInput)
{
  auto weight = Weight::create(parseInput(weightInput));

  if (!weight)
    // handleInvalidWeightError(weight);
    return;

  OptionalGrade grade = std::nullopt;

  if (gradeInput != "") // if the user inputted a grade
  {
    auto gradeResult = Grade::create(parseInput(gradeInput));

    if (!gradeResult)
      // handleInvalidGradeError(gradeResult);
      return;

    grade = *gradeResult;
  }

  auto result = manager.addAssessment(code, name, *weight, grade);

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

void updateGradeCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& gradeInput)
{
  auto grade = Grade::create(parseInput(gradeInput));

  if (!grade)
    // handleInvalidGradeError(grade);
    return;

  auto oldGrade = manager.updateGrade(code, name, *grade);
  if (!oldGrade)
  {
    handleUpdateGradeError(oldGrade, code, name);
    return;
  }

  if (*oldGrade == std::nullopt)
    std::cout << "Added grade " << gradeToPercent(*grade) << " to assessment " << name << " in " << code << std::endl;
  else
    std::cout << "Grade for " << name << " in " << code << " updated from " << gradeToPercent(**oldGrade) << " to " << gradeToPercent(*grade) << std::endl;
}

void showGradeCallback(Manager& manager, bool showScore, bool showExact, const std::string& code, const std::string& name)
{
  if (name == "") // If no assessment name was given, show the grade in the given course
  {
    auto result = manager.getCurrentGrade(code);
    if (!result)
    {
      handleShowCourseGradeError(result, code);
      return;
    }

    if (*result == std::nullopt)
      std::cout << "Cannot calulate grade. No marks have been entered for " << code << std::endl;

    double grade = (**result).ratio();

    if (not showExact)
      grade = round(grade);

    std::cout << "Current grade in " << code << " is " << grade << "%" << std::endl;
  }
  else
  {
    auto gradeResult = manager.getGrade(code, name);

    if (!gradeResult)
      //handleShowAssessmentGradeError(gradeResult, code, name);
      return;

    if (*gradeResult == std::nullopt)
    {
      std::cout << "Assessment " << name << " in " << code << " has not been completed" << std::endl;
      return;
    }

    std::string grade;

    if (showScore)
      grade = gradeToScore(**gradeResult);
    else
      grade = gradeToPercent(**gradeResult);

    std::cout << "Grade for " << name << " in " << code << " is " << grade << "%" << std::endl;
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
