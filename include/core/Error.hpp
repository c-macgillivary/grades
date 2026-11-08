#ifndef ERROR_HPP
#define ERROR_HPP

#include <expected>

enum class Error
{
  ClassNotFound,
  ClassAlreadyExists,
  
  AssessmentNotFound,
  AssessmentAlreadyExists
};

template <typename T>
using Result = std::expected<T, Error>;

#endif // !ERROR_HPP
