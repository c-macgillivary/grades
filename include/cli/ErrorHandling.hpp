#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <string>

#include "core/Error.hpp"
#include "core/Grade.hpp"

void handleAddCourseError(const Result<void>& result, const std::string& code);
void handleRemoveCourseError(const Result<void>& result, const std::string& code);

void handleAddAssessmentError(const Result<void>& result, const std::string& code, const std::string& name);
void handleRemoveAssessmentError(const Result<void>& result, const std::string& code, const std::string& name);

void handleUpdateGradeError(const Result<OptionalGrade>& result, const std::string& code, const std::string& name);
void handleShowCourseGradeError(const Result<OptionalGrade>& result, const std::string& code);
void handleShowAssessmentGradeError(const Result<OptionalGrade>& result, const std::string& code, const std::string& name);

#endif // !ERROR_HANDLING_HPP
