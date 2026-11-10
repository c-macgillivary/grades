#include "core/Calculate.hpp"
#include "core/Grade.hpp"

namespace Calculate
{

Result<OptionalGrade> currentGrade(const Course& course)
{
  const std::unordered_map<std::string, Assessment>& assessments = course.getAssessments();

  double achievedScore = 0.0;
  double assessedWeight = 0.0;

  for (auto [name, a] : assessments)
  {
    auto grade = a.getGrade();
    if (!grade)
      continue;

    achievedScore += grade->ratio();

    Weight weight = a.getWeight();

    assessedWeight += weight.value();
  }

  auto current = Grade::create(achievedScore, assessedWeight);
  if (!current)
    return fail(Error::InvalidCurrentGrade);

  if (assessedWeight == 0.0) // If no assessments have been graded
    return std::nullopt;

  return current;
}

}; // namespace Calculate
