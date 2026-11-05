#ifndef CLASS_H
#define CLASS_H

#include <vector>

#include "core/Assessment.hpp"

class Class
{
private:
  std::vector<Assessment> _assessments;
  std::string _code;

public:
  explicit Class(std::string code = "");

  const std::string& getCode() const
  {
    return _code;
  }

  friend void to_json(nlohmann::json& j, const Class& c);
  friend void from_json(const nlohmann::json& j, Class& c);
};

#endif // !CLASS_H
