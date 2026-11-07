#include <fstream>
#include <nlohmann/json.hpp>

#include "core/Config.hpp"
#include "core/Manager.hpp"

Manager::Manager()
{
  load();
}

void Manager::addClass(const std::string& code)
{
  _classes[code] = Class(code);
}

int Manager::removeClass(const std::string& code)
{
  if (_classes.erase(code))
    return 0;
  // TODO add error handling
  return -1;
}

int Manager::addAssessment(const std::string& code, const std::string& name, double weight, const std::string& grade)
{
  if (not _classes.contains(code))
    return -1;

  return _classes[code].addAssessment(name, weight, grade);
}

int Manager::removeAssessment(const std::string& code, const std::string& name)
{
  if (not _classes.contains(code))
    return -1;

  return _classes[code].removeAssessment(name);
}

std::string Manager::updateGrade(const std::string& code, const std::string& name, const std::string& grade)
{
  if (not _classes.contains(code))
    return "";
  return _classes[code].updateGrade(name, grade);
}

double Manager::getPercentage(const std::string& code, const std::string& name) const
{
  if (not _classes.contains(code))
    return -1;
  return _classes.at(code).getPercentage(name);
}

std::string Manager::getScore(const std::string& code, const std::string& name) const
{
  if (not _classes.contains(code))
    return "";
  return _classes.at(code).getScore(name);
}

int Manager::save()
{
  std::filesystem::create_directories(Config::DATA_DIR);
  std::ofstream file(Config::DATA_DIR / "classes.json");
  nlohmann::json data = _classes;

  if (!file.is_open())
    // TODO add error handling
    return -1;

  file << data.dump(4);

  return 0;
}

int Manager::load()
{
  std::ifstream file(Config::DATA_DIR / "classes.json");

  if (!file.is_open())
    return -1; // No saved data yet

  nlohmann::json data;
  file >> data;

  data.get_to(_classes);
  return 0;
}
