#include <fstream>
#include <nlohmann/json.hpp>

#include "ClassManager.hpp"
#include "Config.hpp"

ClassManager::ClassManager()
{
  load();
}

void ClassManager::addClass(std::string code)
{
  _classes[code] = Class(code);
}

int ClassManager::removeClass(std::string code)
{
  if (_classes.erase(code))
    return 0;
  // TODO add error handling
  return -1;
}

int ClassManager::save()
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

int ClassManager::load()
{
  std::ifstream file(Config::DATA_DIR / "classes.json");

  if (!file.is_open())
    return -1; // No saved data yet

  nlohmann::json data;
  file >> data;

  data.get_to(_classes);
  return 0;
}
