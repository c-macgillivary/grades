#include "core/Course.hpp"

Course::Course(const std::string& code) :
  _code(code)
{
}

int Course::addAssessment(const std::string& name, const std::string& weight, const std::string& grade)
{
  // try_emplace returns a std::pair with the second being a bool
  // true if inserted, false if the key already existed
  if (_assessments.try_emplace(name, name, weight, grade).second)
    return 0;
  return -1;
}

int Course::removeAssessment(const std::string& name)
{
  if (_assessments.erase(name))
    return 0;
  return -1;
}

std::string Course::updateGrade(const std::string& name, const std::string& grade)
{
  if (not _assessments.contains(name))
    return "";
  return _assessments[name].setGrade(grade);
}

double Course::getPercentage(const std::string& name) const
{
  if (_assessments.contains(name))
    return _assessments.at(name).getPercentage();
  return -1.0;
}

std::string Course::getScore(const std::string& name) const
{
  if (_assessments.contains(name))
    return _assessments.at(name).getScore();
  return "";
}

void to_json(nlohmann::json& j, const Course& c)
{
  j = {
    { "code", c._code },
    { "assessments", c._assessments }
  };
}

void from_json(const nlohmann::json& j, Course& c)
{
  j.at("code").get_to(c._code);
  j.at("assessments").get_to(c._assessments);
}
