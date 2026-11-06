#include "core/Class.hpp"

Class::Class(const std::string& code) :
  _code(code)
{
}

int Class::addAssessment(const std::string& name, double weight, double grade)
{
  // try_emplace returns a std::pair with the second being a bool
  // true if inserted, false if the key already existed
  if (_assessments.try_emplace(name, name, weight, grade).second)
    return 0;
  return -1;
}

int Class::removeAssessment(const std::string& name)
{
  if (_assessments.erase(name))
    return 0;
  return -1;
}

int Class::updateGrade(const std::string& name, double grade)
{
  if (not _assessments.contains(name))
    return -1;
  return _assessments[name].setGrade(grade);
}

double Class::getGrade(const std::string& name) const
{
  if (not _assessments.contains(name))
    return -1;
  return _assessments.at(name).getGrade();
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
