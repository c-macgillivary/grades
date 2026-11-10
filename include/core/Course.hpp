#ifndef COURSE_HPP
#define COURSE_HPP

#include "core/Assessment.hpp"
#include "core/Error.hpp"
#include "core/Grade.hpp"

class Course
{
private:
  std::unordered_map<std::string, Assessment> _assessments;
  std::string _code;

public:
  explicit Course(const std::string& code);

  const std::string& getCode() const
  {
    return _code;
  }

  const std::unordered_map<std::string, Assessment>& getAssessments() const
  {
    return _assessments;
  }

  Result<OptionalGrade> getGrade(const std::string& name) const;
  Result<OptionalGrade> updateGrade(const std::string& name, const Grade& grade);

  Result<void> addAssessment(const Assessment& a);
  Result<void> addAssessment(const std::string& name, const Weight& weight, const OptionalGrade& grade);
  Result<void> removeAssessment(const std::string& name);

  friend void to_json(nlohmann::json& j, const Course& c);
};

Result<Course> courseFromJson(const nlohmann::json& j);

#endif // !COURSE_HPP
