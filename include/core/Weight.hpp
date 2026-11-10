#ifndef WEIGHT_HPP
#define WEIGHT_HPP

#include <nlohmann/json.hpp>

#include "core/Error.hpp"

class Weight
{
private:
  double _part;
  double _total;

  Weight(double part, double total) :
    _part(part),
    _total(total)
  {
  }

public:
  static Result<Weight> create(double part, double total);
  static Result<Weight> create(std::pair<double, double> ratio);

  double numerator() const
  {
    return _part;
  }

  double denominator() const
  {
    return _total;
  }

  double value() const
  {
    return _part / _total;
  }

  friend void to_json(nlohmann::json& j, const Weight& g);
};

Result<Weight> weightFromJson(const nlohmann::json& j);

#endif // !WEIGHT_HPP
