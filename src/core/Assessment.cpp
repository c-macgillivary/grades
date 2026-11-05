#include "core/Assessment.hpp"

Assessment::Assessment(std::string name, double weight, double grade) :
  _name(name),
  _weight(weight),
  _grade(grade)
{
}

void to_json(nlohmann::json& j, const Assessment& a)
{
    j = {
        {"name", a._name},
        {"grade", a._grade},
        {"weight", a._weight}
    };
}

void from_json(const nlohmann::json& j, Assessment& a)
{
    j.at("name").get_to(a._name);
    j.at("weight").get_to(a._weight);
    j.at("grade").get_to(a._grade);
}
