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

  void addClass(const std::string& code);
  int removeClass(const std::string& code);

  int addAssessment(const std::string& code, const std::string& name, double weight, double grade);
  int removeAssessment(const std::string& code, const std::string& name);

  int updateGrade(const std::string& code, const std::string& name, double grade);
  int getGrade(const std::string& code, const std::string& name) const;

  const std::unordered_map<std::string, Class>& getClasses() const
  {
    return _classes;
  }

  int save();

  int load();
};

#endif // !MANAGER_HPP
