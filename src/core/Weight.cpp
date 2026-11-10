#include <nlohmann/json.hpp>

#include "core/Weight.hpp"
#include "core/Error.hpp"

Result<Weight> Weight::create(double part, double total)
{
  if (total <= 0 or part < 0 or part > total)
    return fail(Error::InvalidWeight);

  return Weight(part, total);
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
        j.at("total").get<double>()
    );
}
