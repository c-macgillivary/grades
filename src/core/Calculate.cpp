#include "core/Calculate.hpp"
#include "core/Grade.hpp"
#include <iostream>

namespace Calculate
{

Result<OptionalGrade> currentGrade(const Course& course)
{
  const std::unordered_map<std::string, Assessment>& assessments = course.getAssessments();

  double achievedScore = 0.0;
  double assessedWeight = 0.0;

  for (auto [name, a] : assessments)
  {
   // std::cout << "Adding " << name << " to grade" << std::endl;
    auto grade = a.getGrade();
    if (!grade)
      continue;

    Weight weight = a.getWeight();

    assessedWeight += weight.value();
    achievedScore += grade->ratio() * weight.value();
   // std::cout << "Grade for " << name << " is " << grade->ratio() << std::endl;
    //std::cout << "Total achived score is " << achievedScore << std::endl;
    //std::cout << "Total assessed weight is " << assessedWeight << std::endl;
  }

  auto current = Grade::create(achievedScore * 100, assessedWeight * 100);
  if (!current)
    return fail(ErrorCode::InvalidCurrentGrade, std::format("{}/{}", achievedScore, assessedWeight));

  if (assessedWeight == 0.0) // If no assessments have been graded
    return std::nullopt;

  return current;
}

}; // namespace Calculate
