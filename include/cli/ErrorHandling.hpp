#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <iostream>
#include <string>
#include <cassert>

#include "core/Error.hpp"

void handleAddCourseError(const Result<void>& result, const std::string& code);
void handleRemoveCourseError(const Result<void>& result, const std::string& code);

void handleAddAssessmentError(const Result<void>& result, const std::string& code, const std::string& name);
void handleRemoveAssessmentError(const Result<void>& result, const std::string& code, const std::string& name);

void handleUpdateGradeError(const Result<std::string>& result, const std::string& code, const std::string& name);
void handleShowCourseGradeError(const Result<double>& result, const std::string& code);

// TODO: remove template and replace with Result<OptionalGrade> after adding Grade class
// then move to ErrorHandling.cpp. and remove declaration of printErrorMessage()
void printErrorMessage(const Error& error, const std::string& context);
template <class T>
void handleShowAssessmentGradeError(const Result<T>& result, const std::string& code, const std::string& name)
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
    assert(false && "Unexpected error returned by Manager::getScore() or Manager::getPercentage()");
    break;
  }

  printErrorMessage(result.error(), context);
}

#endif // !ERROR_HANDLING_HPP
