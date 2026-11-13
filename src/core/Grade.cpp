#include <nlohmann/json.hpp>

#include "core/Error.hpp"
#include "core/Grade.hpp"

Result<Grade> Grade::create(double part, double total)
{
  if (total <= 0 or part < 0)
    return fail(ErrorCode::InvalidGrade, std::format("{}/{}", part, total));

  return Grade(part, total);
}

Result<Grade> Grade::create(std::pair<double, double> ratio)
{
  if (ratio.first < 0 or ratio.second <= 0)
    return fail(ErrorCode::InvalidGrade, std::format("{}/{}", ratio.first, ratio.second));

  return Grade(ratio.first, ratio.second);
}

void to_json(nlohmann::json& j, const OptionalGrade& g)
{
  if (g)
    j = {
      { "part", g->_part },
      { "total", g->_total }
    };
  else
    j = nullptr;
}

Result<OptionalGrade> gradeFromJson(const nlohmann::json& j)
{
  if (j.is_null())
    return OptionalGrade{};
  return Grade::create(
      j.at("part").get<double>(),
      j.at("total").get<double>());
}
