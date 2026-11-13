#include <nlohmann/json.hpp>

#include "core/Error.hpp"
#include "core/Weight.hpp"

Result<Weight> Weight::create(double part, double total)
{
  if (total <= 0 or part < 0 or part > total)
    return fail(ErrorCode::InvalidWeight, std::format("{}/{}", part, total));

  return Weight(part, total);
}

Result<Weight> Weight::create(std::pair<double, double> ratio)
{
  if (ratio.first < 0 or ratio.second <= 0)
    return fail(ErrorCode::InvalidWeight, std::format("{}/{}", ratio.first, ratio.second));

  return Weight(ratio.first, ratio.second);
}

void to_json(nlohmann::json& j, const Weight& g)
{
  j = {
    { "part", g._part },
    { "total", g._total }
  };
}

Result<Weight> weightFromJson(const nlohmann::json& j)
{
  return Weight::create(
      j.at("part").get<double>(),
      j.at("total").get<double>());
}
