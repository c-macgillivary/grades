#ifndef ERROR_HPP
#define ERROR_HPP

#include <expected>

enum class ErrorCode
{
  CourseNotFound,
  CourseAlreadyExists,

  AssessmentNotFound,
  AssessmentAlreadyExists,

  CouldNotSaveDataToFile,

  InvalidGrade,
  InvalidCurrentGrade,
  InvalidWeight,

  DuplicateAssessmentInJSON,
  DuplicateCourseInJSON
};

template <typename T>
using Result = std::expected<T, Error>;

inline std::unexpected<Error> fail(ErrorCode code)
{
  return std::unexpected<Error>(code);
}

#endif // !ERROR_HPP
