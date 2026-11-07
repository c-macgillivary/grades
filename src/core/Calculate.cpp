#include "core/Calculate.hpp"

double currentGrade(const Course& course)
{
  const std::unordered_map<std::string, Assessment>& assessments = course.getAssessments();

  double achievedScore = 0.0;
  double assessedWeight = 0.0;

  for (auto [name, a] : assessments)
  {
    achievedScore += a.getPercentage() * a.getExactWeight() / 100.0;
    assessedWeight += a.getExactWeight() / 100.0;
  }

  return achievedScore / assessedWeight;
}
