#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include "Assessment.hpp"

class Class
{
private:
  std::vector<Assessment> _assessments;
  std::string _code;
public:
  explicit Class(std::string code);
};

#endif // !CLASS_H
