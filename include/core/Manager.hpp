#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

#include "core/Config.hpp"
#include "core/Course.hpp"

class Manager
{
private:
  std::unordered_map<std::string, Course> _courses;
  bool _clean = false;

public:
  Manager();

  int addCourse(const std::string& code);
  int removeCourse(const std::string& code);

  int addAssessment(const std::string& code, const std::string& name, const std::string& weight, const std::string& grade = "");
  int removeAssessment(const std::string& code, const std::string& name);

  std::string updateGrade(const std::string& code, const std::string& name, const std::string& grade);
  double getPercentage(const std::string& code, const std::string& name) const;
  std::string getScore(const std::string& code, const std::string& name) const;
  double getCurrentGrade(const std::string& code) const;

  std::string dump() const;
  void clean();

  const std::unordered_map<std::string, Course>& getCourses() const
  {
    return _courses;
  }

  static std::string getPath()
  {
    return Config::DATA_DIR.string() + "/courses.json";
  }

  int save();

  int load();
};

#endif // !MANAGER_HPP
