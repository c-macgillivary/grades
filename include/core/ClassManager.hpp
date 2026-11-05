#ifndef CLASS_MANAGER_HPP
#define CLASS_MANAGER_HPP

#include <string>

#include "core/Class.hpp"

class ClassManager
{
private:
  std::unordered_map<std::string, Class> _classes;

public:
  ClassManager();

  void addClass(std::string code);
  int removeClass(std::string code);

  const std::unordered_map<std::string, Class> getClasses() const
  {
    return _classes;
  }

  int save();

  int load();
};

#endif // !CLASS_MANAGER_HPP
