#include "Class.hpp"

Class::Class(std::string code) :
  _code(code)
{
}

void to_json(nlohmann::json& j, const Class& c)
{
  j = {
    { "code", c._code },
    { "assessments", c._assessments }
  };
}

void from_json(const nlohmann::json& j, Class& c)
{
    j.at("code").get_to(c._code);
    j.at("assessments").get_to(c._assessments);
}
