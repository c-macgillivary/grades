#ifndef CLASS_H
#define CLASS_H

#include "core/Assessment.hpp"

class Class
{
private:
  std::unordered_map<std::string, Assessment> _assessments;
  std::string _code;

public:
  explicit Class(const std::string& code = "");

  const std::string& getCode() const
  {
    return _code;
  }

  int addAssessment(const std::string& name, double weight, double grade = -1.0);
  int removeAssessment(const std::string& name);

  // Returns -1 for an error, or the old value of the grade
  int updateGrade(const std::string& name, double grade);

  double getGrade(const std::string& name) const;

  friend void to_json(nlohmann::json& j, const Class& c);
  friend void from_json(const nlohmann::json& j, Class& c);
};

#endif // !CLASS_H
