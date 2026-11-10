#include <nlohmann/json.hpp>

#include "core/Error.hpp"
#include "core/Grade.hpp"

Result<Grade> Grade::create(double part, double total)
{
  if (total <= 0 or part < 0)
    return fail(Error::InvalidGrade);

  return Grade(part, total);
}

Result<Grade> Grade::create(std::pair<double, double> ratio)
{
  if (ratio.first < 0 or ratio.second <= 0)
    return fail(Error::InvalidGrade);

  return Grade(ratio.first, ratio.second);
}

void to_json(nlohmann::json& j, const Grade& g)
{
  j = {
    { "part", g._part },
    { "total", g._total }
  };
}

Result<Grade> gradeFromJson(const nlohmann::json& j)
{
  return Grade::create(
      j.at("part").get<double>(),
      j.at("total").get<double>());
}
