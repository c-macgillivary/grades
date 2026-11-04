#ifndef CLASS_MANAGER_HPP
#define CLASS_MANAGER_HPP

#include <vector>

#include "Class.hpp"

class ClassManager
{
private:
  std::vector<Class> _classes;
public:
  void addClass(std::string name);

  int save();
};

#endif // !CLASS_MANAGER_HPP
