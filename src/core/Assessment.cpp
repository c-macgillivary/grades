#include "core/Assessment.hpp"
#include "core/Grade.hpp"

Assessment::Assessment(const std::string& name, const Weight& weight, const OptionalGrade& grade) :
  _name(name),
  _weight(weight),
  _grade(grade)
{
}

OptionalGrade Assessment::setGrade(const Grade& grade)
{
  OptionalGrade og = _grade;
  _grade = grade;
  return og;
}

void to_json(nlohmann::json& j, const Assessment& a)
{
  j = {
    { "name", a._name },
    { "grade", a._grade },
    { "weight", a._weight }
  };
}

Result<Assessment> assessmentFromJson(const nlohmann::json& j)
{
  auto weight = weightFromJson(j.at("weight"));

  if (!weight)
    return fail(weight.error());

  OptionalGrade grade = std::nullopt;

  if (!j.at("grade").is_null())
  {
    auto result = gradeFromJson(j.at("grade"));

    if (!result)
      return fail(result.error());

    grade = *result;
  }

  return Assessment(j.at("name").get<std::string>(), *weight, grade);
}
