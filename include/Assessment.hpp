#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include <string>

class Assessment
{
private:
  std::string _name;
  double _weight;
  double _grade = -1;
public:
  Assessment(std::string name, double weight);
};

#endif // !ASSESSMENT_H
