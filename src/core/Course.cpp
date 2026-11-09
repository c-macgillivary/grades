#include "core/Course.hpp"
#include "core/Error.hpp"

Course::Course(const std::string& code) :
  _code(code)
{
}

Result<void> Course::addAssessment(const std::string& name, const std::string& weight, const std::string& grade)
{
  if (_assessments.contains(name))
    return fail(Error::AssessmentAlreadyExists);

  _assessments.try_emplace(name, name, weight, grade);

  return {};
}

Result<void> Course::removeAssessment(const std::string& name)
{
  if (not _assessments.contains(name))
    return fail(Error::AssessmentNotFound);

  _assessments.erase(name);

  return {};
}

Result<std::string> Course::updateGrade(const std::string& name, const std::string& grade)
{
  if (not _assessments.contains(name))
    return fail(Error::AssessmentNotFound);

  return _assessments[name].setGrade(grade);
}

Result<double> Course::getPercentage(const std::string& name) const
{
  if (not _assessments.contains(name))
    return fail(Error::AssessmentNotFound);

  return _assessments.at(name).getPercentage();
}

Result<std::string> Course::getScore(const std::string& name) const
{
  if (not _assessments.contains(name))
    return fail(Error::AssessmentNotFound);

  return _assessments.at(name).getScore();
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
