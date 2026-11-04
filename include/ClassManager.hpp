#ifndef CLASS_MANAGER_HPP
#define CLASS_MANAGER_HPP

#include <string>

#include "Class.hpp"

class ClassManager
{
private:
  std::unordered_map<std::string, Class> _classes;

public:
  ClassManager();

  void addClass(std::string code);
  int removeClass(std::string code);

  int save();

  int load();
};

#endif // !CLASS_MANAGER_HPP
