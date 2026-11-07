#ifndef CLASS_H
#define CLASS_H

#include "core/Assessment.hpp"

class Class
{
private:
  std::unordered_map<std::string, Assessment> _assessments;
  std::string _code;

public:
  Class() = default;
  explicit Class(const std::string& code);

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

  friend void to_json(nlohmann::json& j, const Class& c);
  friend void from_json(const nlohmann::json& j, Class& c);
};

#endif // !CLASS_H
