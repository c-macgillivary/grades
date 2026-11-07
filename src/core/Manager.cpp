#include <fstream>
#include <nlohmann/json.hpp>

#include "core/Config.hpp"
#include "core/Manager.hpp"

Manager::Manager()
{
  load();
}

int Manager::addCourse(const std::string& code)
{
  if (_courses.contains(code))
    return -1;

  _courses[code] = Course(code);
  return 0;
}

int Manager::removeCourse(const std::string& code)
{
  if (_courses.erase(code))
    return 0;
  // TODO add error handling
  return -1;
}

int Manager::addAssessment(const std::string& code, const std::string& name, double weight, const std::string& grade)
{
  if (not _courses.contains(code))
    return -1;

  return _courses[code].addAssessment(name, weight, grade);
}

int Manager::removeAssessment(const std::string& code, const std::string& name)
{
  if (not _courses.contains(code))
    return -1;

  return _courses[code].removeAssessment(name);
}

std::string Manager::updateGrade(const std::string& code, const std::string& name, const std::string& grade)
{
  if (not _courses.contains(code))
    return "";
  return _courses[code].updateGrade(name, grade);
}

double Manager::getPercentage(const std::string& code, const std::string& name) const
{
  if (not _courses.contains(code))
    return -1;
  return _courses.at(code).getPercentage(name);
}

std::string Manager::getScore(const std::string& code, const std::string& name) const
{
  if (not _courses.contains(code))
    return "";
  return _courses.at(code).getScore(name);
}

int Manager::save()
{
  std::filesystem::create_directories(Config::DATA_DIR);
  std::ofstream file(Config::DATA_DIR / "courses.json");
  nlohmann::json data = _courses;

  if (!file.is_open())
    // TODO add error handling
    return -1;

  file << data.dump(4);

  return 0;
}

int Manager::load()
{
  std::ifstream file(Config::DATA_DIR / "courses.json");

  if (!file.is_open())
    return -1; // No saved data yet

  nlohmann::json data;
  file >> data;

  data.get_to(_courses);
  return 0;
}
