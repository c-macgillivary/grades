#ifndef ASSESSMENT_HPP
#define ASSESSMENT_HPP

#include <nlohmann/json.hpp>
#include <string>

#include "core/Grade.hpp"
#include "core/Weight.hpp"

class Assessment
{
private:
  std::string _name = "";
  OptionalGrade _grade;
  Weight _weight;

public:
  Assessment(const std::string& name, const Weight& weight, const OptionalGrade& grade);

  const std::string& getName() const
  {
    return _name;
  }

  const OptionalGrade& getGrade() const
  {
    return _grade;
  }

  const Weight& getWeight() const
  {
    return _weight;
  }

  // Returns the previous value of the grade
  OptionalGrade setGrade(const Grade& grade);

  friend void to_json(nlohmann::json& j, const Assessment& a);
};

Result<Assessment> assessmentFromJson(const nlohmann::json& j);

#endif // !ASSESSMENT_HPP
