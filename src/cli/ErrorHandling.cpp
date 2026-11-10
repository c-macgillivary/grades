#include <cassert>
#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "core/Error.hpp"
#include "core/Grade.hpp"

void printErrorMessage(const Error& error, const std::string& context)
{
  switch (error)
  {
  case Error::CourseAlreadyExists:
    std::cout << "Course " << context << " already exists" << std::endl;
    break;
  case Error::CourseNotFound:
    std::cout << "Course " << context << " does not exist" << std::endl;
    break;
  case Error::AssessmentAlreadyExists:
    std::cout << "Assessment " << context << " already exists" << std::endl;
    break;
  case Error::AssessmentNotFound:
    std::cout << "Assessment " << context << " does not exist" << std::endl;
    break;
  case Error::CouldNotSaveDataToFile:
    std::cout << "Could not open file " << context << " for writing" << std::endl;
    break;
  }
}

void handleAddCourseError(const Result<void>& result, const std::string& code)
{
  assert(!result);

  std::cout << "Could not add course " << code << std::endl;
  printErrorMessage(result.error(), code);
}

void handleRemoveCourseError(const Result<void>& result, const std::string& code)
{
  assert(!result);

  std::cout << "Could not remove course " << code << std::endl;
  printErrorMessage(result.error(), code);
}

void handleAddAssessmentError(const Result<void>& result, const std::string& code, const std::string& name)
{
  assert(!result);

  std::cout << "Could not add assessment " << name << " to " << code << std::endl;
  std::string context = "";

  switch (result.error())
  {
  case Error::AssessmentAlreadyExists:
    context = name;
    break;
  case Error::CourseNotFound:
    context = code;
    break;
  default:
    assert(false && "Unexpected error returned by Manager::addAssessment()");
    break;
  }
  printErrorMessage(result.error(), context);
}

void handleRemoveAssessmentError(const Result<void>& result, const std::string& code, const std::string& name)
{
  assert(!result);

  std::cout << "Could not remove assessment " << name << " from " << code << std::endl;
  std::string context = "";

  switch (result.error())
  {
  case Error::AssessmentNotFound:
    context = name;
    break;
  case Error::CourseNotFound:
    context = code;
    break;
  default:
    assert(false && "Unexpected error returned by Manager::removeAssessment()");
    break;
  }

  printErrorMessage(result.error(), context);
}

void handleUpdateGradeError(const Result<OptionalGrade>& result, const std::string& code, const std::string& name)
{
  assert(!result);

  std::cout << "Could not update grade for " << name << " in " << code << std::endl;
  std::string context = "";

  switch (result.error())
  {
  case Error::AssessmentNotFound:
    context = name;
    break;
  case Error::CourseNotFound:
    context = code;
    break;
  default:
    assert(false && "Unexpected error returned by Manager::updateGrade()");
    break;
  }

  printErrorMessage(result.error(), context);
}

void handleShowCourseGradeError(const Result<OptionalGrade>& result, const std::string& code)
{
  assert(!result);

  std::cout << "Could not show current grade in " << code << std::endl;
  printErrorMessage(result.error(), code);
}

void handleShowAssessmentGradeError(const Result<Grade>& result, const std::string& code, const std::string& name)
{
  assert(!result);

  std::cout << "Could not show grade for " << name << " in " << code << std::endl;
  std::string context = "";

  switch (result.error())
  {
  case Error::AssessmentNotFound:
    context = name;
    break;
  case Error::CourseNotFound:
    context = code;
    break;
  default:
    assert(false && "Unexpected error returned by Manager::getGrade()");
    break;
  }

  printErrorMessage(result.error(), context);
}
