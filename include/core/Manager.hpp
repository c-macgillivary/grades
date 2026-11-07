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

  int addAssessment(const std::string& code, const std::string& name, double weight, const std::string& grade = "");
  int removeAssessment(const std::string& code, const std::string& name);

  std::string updateGrade(const std::string& code, const std::string& name, const std::string& grade);
  double getPercentage(const std::string& code, const std::string& name) const;
  std::string getScore(const std::string& code, const std::string& name) const;

  const std::unordered_map<std::string, Class>& getClasses() const
  {
    return _classes;
  }

  int save();

  int load();
};

#endif // !MANAGER_HPP
