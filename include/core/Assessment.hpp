#ifndef ASSESSMENT_HPP
#define ASSESSMENT_HPP

#include <nlohmann/json.hpp>
#include <string>

std::pair<double, double> parseGrade(const std::string& input);
std::string unparseGrade(double score, double total, bool usePercent = false);

class Assessment
{
private:
  std::string _name = "";
  double _weight = 0.0;
  int _total = -1;
  double _score = -1;

public:
  Assessment() = default;
  Assessment(const std::string& name, double weight, const std::string& grade);

  double getPercentage() const
  {
    if (_total <= 0 or _score < 0)
      return -1;
    return _score / _total * 100.0;
  }

  std::string getScore() const
  {
    return unparseGrade(_score, _total);
  }

  // Returns the previous value of the grade
  std::string setGrade(const std::string& grade);

  friend void to_json(nlohmann::json& j, const Assessment& a);
  friend void from_json(const nlohmann::json& j, Assessment& a);
};

#endif // !ASSESSMENT_HPP
