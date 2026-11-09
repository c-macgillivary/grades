#ifndef ERROR_HPP
#define ERROR_HPP

#include <expected>

enum class Error
{
  CourseNotFound,
  CourseAlreadyExists,
  
  AssessmentNotFound,
  AssessmentAlreadyExists,

  CouldNotSaveDataToFile
};

template <typename T>
using Result = std::expected<T, Error>;

inline std::unexpected<Error> fail(Error e)
{
    return std::unexpected<Error>(e);
}

#endif // !ERROR_HPP
