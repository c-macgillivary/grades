#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

#include "core/Config.hpp"
#include "core/Course.hpp"
#include "core/Error.hpp"
#include "core/Grade.hpp"

class Manager
{
private:
  std::unordered_map<std::string, Course> _courses;
  bool _clean = false;

public:
  Result<void> addCourse(const std::string& code);
  Result<void> addCourse(const Course& course);
  Result<void> removeCourse(const std::string& code);

  Result<void> addAssessment(const std::string& code, const std::string& name, const Weight& weight, const OptionalGrade& grade);
  Result<void> removeAssessment(const std::string& code, const std::string& name);

  Result<OptionalGrade> updateGrade(const std::string& code, const std::string& name, const Grade& grade);
  Result<OptionalGrade> getGrade(const std::string& code, const std::string& name) const;
  Result<OptionalGrade> getCurrentGrade(const std::string& code) const;

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

  Result<void> load();
};

#endif // !MANAGER_HPP
