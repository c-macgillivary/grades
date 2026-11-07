#include "core/Assessment.hpp"

std::pair<double, double> parseValue(const std::string& input)
{
  if (input == "")
    return { -1, -1 };
  size_t slashPos = input.find('/');
  size_t percentPos = input.find('%');

  if (slashPos != std::string::npos) // If it contains a "/" (is a score)
    return {
      std::stod(input.substr(0, slashPos)),
      std::stod(input.substr(slashPos + 1))
    };

  if (percentPos != std::string::npos) // If it contains a "%", remove it
    return { std::stod(input.substr(0, input.length() - 1)), 100 };

  return { std::stod(input), 100 };
}

std::string unparseValue(double score, double total, bool usePercent)
{
  if (score < 0 or total <= 0)
    return "";
  if (total == 100 and usePercent)
    return std::format("{}%", score);
  return std::format("{}/{}", score, total);
}

Assessment::Assessment(const std::string& name, const std::string& weight, const std::string& grade) :
  _name(name)
{
  auto [score, total] = parseValue(grade);
  _score = score;
  _total = total;
  auto [partWeight, totalWeight] = parseValue(weight);
  _partWeight = partWeight;
  _totalWeight = totalWeight;
}

std::string Assessment::setGrade(const std::string& grade)
{
  std::string old = unparseValue(_score, _total, true);
  if (old == "")
    old = "new";

  auto [score, total] = parseValue(grade);
  _score = score;
  _total = total;

  return old;
}

void to_json(nlohmann::json& j, const Assessment& a)
{
  j = {
    { "name", a._name },
    { "part weight", a._partWeight },
    { "total weight", a._totalWeight },
    { "total", a._total },
    { "score", a._score }
  };
}

void from_json(const nlohmann::json& j, Assessment& a)
{
  j.at("name").get_to(a._name);
  j.at("part weight").get_to(a._partWeight);
  j.at("total weight").get_to(a._totalWeight);
  j.at("total").get_to(a._total);
  j.at("score").get_to(a._score);
}
