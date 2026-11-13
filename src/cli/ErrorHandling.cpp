#include <cassert>
#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "core/Error.hpp"

void printErrorMessage(const ErrorCode& error, const std::string& context)
{
  std::cerr << "Error: ";
  switch (error)
  {
  case ErrorCode::CourseAlreadyExists:
    std::cerr << "Course " << context << " already exists" << std::endl;
    break;
  case ErrorCode::CourseNotFound:
    std::cerr << "Course " << context << " does not exist" << std::endl;
    break;
  case ErrorCode::AssessmentAlreadyExists:
    std::cerr << "Assessment " << context << " already exists" << std::endl;
    break;
  case ErrorCode::AssessmentNotFound:
    std::cerr << "Assessment " << context << " does not exist" << std::endl;
    break;
  case ErrorCode::CouldNotSaveDataToFile:
    std::cerr << "Could not open file " << context << " for writing" << std::endl;
    break;
  case ErrorCode::InvalidWeight:
    std::cerr << "Invalid weight: " << context << std::endl;
    break;
  case ErrorCode::InvalidGrade:
    std::cerr << "Invalid grade: " << context << std::endl;
    break;
  case ErrorCode::InvalidCurrentGrade:
    std::cerr << "Invalid current grade: " << context << std::endl;
    break;
  case ErrorCode::DuplicateAssessmentInJSON:
    std::cerr << "Invalid save file. Duplicate assessment " << context << std::endl; 
    break;
  case ErrorCode::DuplicateCourseInJSON:
    std::cerr << "Invalid save file. Duplicate course " << context << std::endl; 
    break;
  }
}
