#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

#include "core/Config.hpp"
#include "core/Course.hpp"
#include "core/Error.hpp"

class Manager
{
private:
  std::unordered_map<std::string, Course> _courses;
  bool _clean = false;

public:

  Result<void> addCourse(const std::string& code);
  Result<void> removeCourse(const std::string& code);

  Result<void> addAssessment(const std::string& code, const std::string& name, const std::string& weight, const std::string& grade = "");
  Result<void> removeAssessment(const std::string& code, const std::string& name);

  Result<std::string> updateGrade(const std::string& code, const std::string& name, const std::string& grade);
  Result<double> getPercentage(const std::string& code, const std::string& name) const;
  Result<std::string> getScore(const std::string& code, const std::string& name) const;
  Result<double> getCurrentGrade(const std::string& code) const;

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

  Result<void> save();

  void load();
};

#endif // !MANAGER_HPP
