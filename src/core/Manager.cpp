#include <fstream>
#include <nlohmann/json.hpp>

#include "core/Calculate.hpp"
#include "core/Config.hpp"
#include "core/Error.hpp"
#include "core/Manager.hpp"

Result<void> Manager::addCourse(const std::string& code)
{
  if (_courses.contains(code))
    return fail(ErrorCode::CourseAlreadyExists);

  _courses.emplace(code, code);

  return {};
}

Result<void> Manager::addCourse(const Course& course)
{
  if (_courses.contains(course.getCode()))
    return fail(ErrorCode::CourseAlreadyExists);

  _courses.emplace(course.getCode(), course);
  return {};
}

Result<void> Manager::removeCourse(const std::string& code)
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  _courses.erase(code);

  return {};
}

Result<void> Manager::addAssessment(const std::string& code, const std::string& name, const Weight& weight, const OptionalGrade& grade)
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  return _courses.at(code).addAssessment(name, weight, grade);
}

Result<void> Manager::removeAssessment(const std::string& code, const std::string& name)
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  return _courses.at(code).removeAssessment(name);
}

Result<OptionalGrade> Manager::updateGrade(const std::string& code, const std::string& name, const Grade& grade)
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  return _courses.at(code).updateGrade(name, grade);
}

Result<OptionalGrade> Manager::getGrade(const std::string& code, const std::string& name) const
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  return _courses.at(code).getGrade(name);
}

Result<OptionalGrade> Manager::getCurrentGrade(const std::string& code) const
{
  if (not _courses.contains(code))
    return fail(ErrorCode::CourseNotFound);

  return Calculate::currentGrade(_courses.at(code));
}

std::string Manager::dump() const
{
  nlohmann::json data = _courses;
  return data.dump(4);
}

void Manager::clean()
{
  std::filesystem::remove(Config::DATA_DIR / "courses.json");
  _clean = true;
}

Result<void> Manager::save() const
{
  if (_clean) // Do not save if the user just cleaned the data
    return {};

  std::filesystem::create_directories(Config::DATA_DIR);
  std::ofstream file(Config::DATA_DIR / "courses.json");
  nlohmann::json data = _courses;

  if (!file.is_open())
    return fail(ErrorCode::CouldNotSaveDataToFile);

  file << data.dump(4);

  return {};
}

Result<void> Manager::load()
{
  std::ifstream file(Config::DATA_DIR / "courses.json");

  if (!file.is_open())
    return {};

  nlohmann::json data;
  file >> data;

  for (const auto& [code, courseJson] : data.items())
  {
    auto course = courseFromJson(courseJson);

    if (!course)
      return fail(course.error());

    if (!addCourse(*course))
      return fail(ErrorCode::DuplicateCourseInJSON);
  }

  return {};
}
