#include "core/Course.hpp"
#include "core/Error.hpp"
#include "core/Grade.hpp"

Course::Course(const std::string& code) :
  _code(code)
{
}

Result<OptionalGrade> Course::getGrade(const std::string& name) const
{
  if (!_assessments.contains(name))
    return fail(ErrorCode::AssessmentNotFound, name);

  return _assessments.at(name).getGrade();
}

Result<OptionalGrade> Course::updateGrade(const std::string& name, const Grade& grade)
{
  if (not _assessments.contains(name))
    return fail(ErrorCode::AssessmentNotFound, name);

  return _assessments.at(name).setGrade(grade);
}

Result<void> Course::addAssessment(const Assessment& a)
{
  if (_assessments.contains(a.getName()))
    return fail(ErrorCode::AssessmentAlreadyExists, a.getName());

  _assessments.emplace(a.getName(), a);
  return {};
}

Result<void> Course::addAssessment(const std::string& name, const Weight& weight, const OptionalGrade& grade)
{
  if (_assessments.contains(name))
    return fail(ErrorCode::AssessmentAlreadyExists, name);

  _assessments.try_emplace(name, name, weight, grade);

  return {};
}

Result<void> Course::removeAssessment(const std::string& name)
{
  if (not _assessments.contains(name))
    return fail(ErrorCode::AssessmentNotFound, name);

  _assessments.erase(name);

  return {};
}

void to_json(nlohmann::json& j, const Course& c)
{
  j = {
    { "code", c._code },
    { "assessments", c._assessments }
  };
}

Result<Course> courseFromJson(const nlohmann::json& j)
{
  Course course(j.at("code").get<std::string>());

  for (const auto& [name, assessmentJson] : j.at("assessments").items())
  {
    auto assessment = assessmentFromJson(assessmentJson);

    if (!assessment)
      return fail(assessment.error());

    if (!course.addAssessment(*assessment))
      return fail(ErrorCode::DuplicateAssessmentInJSON, name);
  }

  return course;
}
