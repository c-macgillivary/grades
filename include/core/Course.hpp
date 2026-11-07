#ifndef COURSE_HPP
#define COURSE_HPP

#include "core/Assessment.hpp"

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

  int addAssessment(const std::string& name, double weight, const std::string& grade = "");
  int removeAssessment(const std::string& name);

  // Returns "" for an error, or the old value of the grade
  std::string updateGrade(const std::string& name, const std::string& grade);

  double getPercentage(const std::string& name) const;
  std::string getScore(const std::string& name) const;

  friend void to_json(nlohmann::json& j, const Course& c);
  friend void from_json(const nlohmann::json& j, Course& c);
};

#endif // !COURSE_HPP
