#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <string>

#include "core/Error.hpp"

void printErrorMessage(const ErrorCode& error, const std::string& context);

#endif // !ERROR_HANDLING_HPP
