#include <algorithm>
#include <string>

#include "core/Utility.hpp"

bool isWhitespace(const std::string& str)
{
  return std::all_of(str.begin(), str.end(),
              [](unsigned char c)
              { return std::isspace(c); });
}
