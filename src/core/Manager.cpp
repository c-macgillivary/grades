#include <fstream>
#include <nlohmann/json.hpp>

#include "core/Manager.hpp"
#include "core/Config.hpp"

Manager::Manager()
{
  load();
}

void Manager::addClass(std::string code)
{
  _classes[code] = Class(code);
}

int Manager::removeClass(std::string code)
{
  if (_classes.erase(code))
    return 0;
  // TODO add error handling
  return -1;
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
