#include <cassert>
#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "core/Error.hpp"

void printErrorMessage(const Error& error)
{
  std::cerr << "Error: ";
  switch (error.code)
  {
  case ErrorCode::CourseAlreadyExists:
    std::cerr << "Course " << error.context << " already exists" << std::endl;
    break;
  case ErrorCode::CourseNotFound:
    std::cerr << "Course " << error.context << " does not exist" << std::endl;
    break;
  case ErrorCode::AssessmentAlreadyExists:
    std::cerr << "Assessment " << error.context << " already exists" << std::endl;
    break;
  case ErrorCode::AssessmentNotFound:
    std::cerr << "Assessment " << error.context << " does not exist" << std::endl;
    break;
  case ErrorCode::CouldNotSaveDataToFile:
    std::cerr << "Could not open file " << error.context << " for writing" << std::endl;
    break;
  case ErrorCode::InvalidWeight:
    std::cerr << "Invalid weight: " << error.context << std::endl;
    break;
  case ErrorCode::InvalidGrade:
    std::cerr << "Invalid grade: " << error.context << std::endl;
    break;
  case ErrorCode::InvalidCurrentGrade:
    std::cerr << "Invalid current grade: " << error.context << std::endl;
    break;
  case ErrorCode::DuplicateAssessmentInJSON:
    std::cerr << "Invalid save file. Duplicate assessment " << error.context << std::endl; 
    break;
  case ErrorCode::DuplicateCourseInJSON:
    std::cerr << "Invalid save file. Duplicate course " << error.context << std::endl; 
    break;
  }
}
