#include <nlohmann/json.hpp>

#include "core/Grade.hpp"
#include "core/Error.hpp"

Result<Grade> Grade::create(double part, double total)
{
  if (total <= 0 or part < 0)
    return fail(Error::InvalidGrade);

  return Grade(part, total);
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
        j.at("total").get<double>()
    );
}
