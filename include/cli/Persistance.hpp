#ifndef PERSISTANCE_HPP
#define PERSISTANCE_HPP

#include "core/Manager.hpp"

// Returns true for a successful load
bool load(Manager& manager);

void save(const Manager& manager);

#endif // !PERSISTANCE_HPP
