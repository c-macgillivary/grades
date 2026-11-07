#include "core/Assessment.hpp"

std::pair<double, double> parseGrade(const std::string& input)
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

std::string unparseGrade(double score, double total, bool usePercent)
{
  if (score < 0 or total <= 0)
    return "";
  if (total == 100 and usePercent)
    return std::format("{}%", score);
  return std::format("{}/{}", score, total);
}

Assessment::Assessment(const std::string& name, double weight, const std::string& grade) :
  _name(name),
  _weight(weight)
{
  auto [score, total] = parseGrade(grade);
  _score = score;
  _total = total;
}

std::string Assessment::setGrade(const std::string& grade)
{
  std::string old = unparseGrade(_score, _total, true);

  auto [score, total] = parseGrade(grade);
  _score = score;
  _total = total;

  return old;
}

void to_json(nlohmann::json& j, const Assessment& a)
{
  j = {
    { "name", a._name },
    { "weight", a._weight },
    { "total", a._total },
    { "score", a._score }
  };
}

void from_json(const nlohmann::json& j, Assessment& a)
{
  j.at("name").get_to(a._name);
  j.at("weight").get_to(a._weight);
  j.at("total").get_to(a._total);
  j.at("score").get_to(a._score);
}
