#include <filesystem>
#include <nlohmann/json.hpp>

#include "ClassManager.hpp"
#include "Config.hpp"

using nlohmann::json;

void ClassManager::addClass(std::string name)
{
  _classes.emplace_back(name);
}

int ClassManager::save()
{
  std::filesystem::path write = Config::DATA_DIR;

  return 0;
}
