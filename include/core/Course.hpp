#ifndef COURSE_HPP
#define COURSE_HPP

#include "core/Assessment.hpp"
#include "core/Error.hpp"

class Course
{
private:
  std::unordered_map<std::string, Assessment> _assessments;
  std::string _code;

public:
  Course() = default;
  explicit Course(const std::string& code);

  const std::string& getCode() const
  {
    return _code;
  }

  const std::unordered_map<std::string, Assessment>& getAssessments() const
  {
    return _assessments;
  }

  Result<double> getPercentage(const std::string& name) const;
  Result<std::string> getScore(const std::string& name) const;

  Result<void> addAssessment(const std::string& name, const std::string& weight, const std::string& grade = "");
  Result<void> removeAssessment(const std::string& name);

  Result<std::string> updateGrade(const std::string& name, const std::string& grade);

  friend void to_json(nlohmann::json& j, const Course& c);
  friend void from_json(const nlohmann::json& j, Course& c);
};

#endif // !COURSE_HPP
