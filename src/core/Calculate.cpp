#include "core/Calculate.hpp"

namespace Calculate
{
double currentGrade(const Course& course)
{
  const std::unordered_map<std::string, Assessment>& assessments = course.getAssessments();

  double achievedScore = 0.0;
  double assessedWeight = 0.0;

  for (auto [name, a] : assessments)
  {
    auto percent = a.getPercentage();
    if (percent < 0)
      continue;
    achievedScore += percent * a.getExactWeight() / 100.0;
    assessedWeight += a.getExactWeight() / 100.0;
  }

  return achievedScore / assessedWeight;
}
}; // namespace Calculate
