#include <iostream>

#include "cli/ErrorHandling.hpp"
#include "cli/Persistance.hpp"

bool load(Manager& manager)
{
  auto result = manager.load();
  if (result)
    return true;

  std::cerr << "Could not load data from file" << std::endl;
  printErrorMessage(result.error(), "");
  return false;
}

void save(const Manager& manager)
{
  if (!manager.save())
    std::cerr << "Could not save data to file" << std::endl;
}
