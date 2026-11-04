#include <fstream>
#include <nlohmann/json.hpp>

#include "ClassManager.hpp"
#include "Config.hpp"

void ClassManager::addClass(std::string name)
{
  _classes.emplace_back(name);
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
