#include <cassert>
#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "core/Error.hpp"

void printErrorMessage(const Error& error, const std::string& context)
{
  std::cerr << "Error: ";
  switch (error)
  {
  case Error::CourseAlreadyExists:
    std::cerr << "Course " << context << " already exists" << std::endl;
    break;
  case Error::CourseNotFound:
    std::cerr << "Course " << context << " does not exist" << std::endl;
    break;
  case Error::AssessmentAlreadyExists:
    std::cerr << "Assessment " << context << " already exists" << std::endl;
    break;
  case Error::AssessmentNotFound:
    std::cerr << "Assessment " << context << " does not exist" << std::endl;
    break;
  case Error::CouldNotSaveDataToFile:
    std::cerr << "Could not open file " << context << " for writing" << std::endl;
    break;
  }
}
