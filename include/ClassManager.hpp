#ifndef CLASS_MANAGER_HPP
#define CLASS_MANAGER_HPP

#include <vector>

#include "Class.hpp"

class ClassManager
{
private:
  std::vector<Class> _classes;
public:
  ClassManager();

  void addClass(std::string name);

  int save();
  
  int load();
};

#endif // !CLASS_MANAGER_HPP
