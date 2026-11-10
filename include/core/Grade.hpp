#ifndef GRADE_HPP
#define GRADE_HPP

#include <nlohmann/json.hpp>
#include <optional>
#include <utility>

#include "core/Error.hpp"

class Grade
{
private:
  double _part;
  double _total;

  Grade(double part, double total) :
    _part(part),
    _total(total)
  {
  }

public:
  static Result<Grade> create(double part, double total);
  static Result<Grade> create(std::pair<double, double> ratio);

  double numerator() const
  {
    return _part;
  }

  double denominator() const
  {
    return _total;
  }

  double ratio() const
  {
    return _part / _total;
  }

  std::pair<double, double> pair() const
  {
    return { _part, _total};
  }

  friend void to_json(nlohmann::json& j, const Grade& g);
};

using OptionalGrade = std::optional<Grade>;

Result<Grade> gradeFromJson(const nlohmann::json& j);

#endif // !GRADE_HPP
