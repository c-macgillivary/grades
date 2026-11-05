#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

#include "core/Class.hpp"

class Manager
{
private:
  std::unordered_map<std::string, Class> _classes;

public:
  Manager();

  void addClass(std::string code);
  int removeClass(std::string code);

  const std::unordered_map<std::string, Class>& getClasses() const
  {
    return _classes;
  }

  int save();

  int load();
};

#endif // !MANAGER_HPP
