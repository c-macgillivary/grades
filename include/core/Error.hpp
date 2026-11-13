#ifndef ERROR_HPP
#define ERROR_HPP

#include <expected>
#include <string>

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

struct Error
{
  ErrorCode code;
  std::string context;
};

template <typename T>
using Result = std::expected<T, Error>;

inline std::unexpected<Error> fail(ErrorCode code, const std::string& context = "")
{
  return std::unexpected<Error>({ code, context });
}

inline std::unexpected<Error> fail(Error e)
{
  return std::unexpected<Error>(e);
}

#endif // !ERROR_HPP
