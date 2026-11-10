#ifndef GRADE_HPP
#define GRADE_HPP

#include <nlohmann/json.hpp>
#include <optional>

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

  double numerator() const
  {
    return _part;
  }

  double denominator() const
  {
    return _total;
  }

  double percentage() const
  {
    return _part / _total;
  }

  friend void to_json(nlohmann::json& j, const Grade& g);
};

using OptionalGrade = std::optional<Grade>;

Result<Grade> gradeFromJson(const nlohmann::json& j);

#endif // !GRADE_HPP
