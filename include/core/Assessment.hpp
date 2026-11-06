#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include <nlohmann/json.hpp>
#include <string>

class Assessment
{
private:
  std::string _name;
  double _weight;
  double _grade;

public:
  explicit Assessment(std::string name = "", double weight = 0.0, double grade = -1);

  // Returns the previous value of the grade
  int setGrade(double grade)
  {
    auto temp = (int)_grade;
    _grade = grade;
    return temp;
  }

  friend void to_json(nlohmann::json& j, const Assessment& a);
  friend void from_json(const nlohmann::json& j, Assessment& a);
};

#endif // !ASSESSMENT_H
