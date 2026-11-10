#ifndef CALCULATE_HPP
#define CALCULATE_HPP

#include "core/Course.hpp"
#include "core/Grade.hpp"

namespace Calculate
{
  Result<OptionalGrade> currentGrade(const Course& course);
};

#endif // !CALCULATE_HPP
