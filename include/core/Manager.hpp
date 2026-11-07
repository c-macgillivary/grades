#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

#include "core/Course.hpp"

class Manager
{
private:
  std::unordered_map<std::string, Course> _courses;

public:
  Manager();

  int addCourse(const std::string& code);
  int removeCourse(const std::string& code);

  int addAssessment(const std::string& code, const std::string& name, double weight, const std::string& grade = "");
  int removeAssessment(const std::string& code, const std::string& name);

  std::string updateGrade(const std::string& code, const std::string& name, const std::string& grade);
  double getPercentage(const std::string& code, const std::string& name) const;
  std::string getScore(const std::string& code, const std::string& name) const;

  const std::unordered_map<std::string, Course>& getCourses() const
  {
    return _courses;
  }

  int save();

  int load();
};

#endif // !MANAGER_HPP
