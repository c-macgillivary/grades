#ifndef CALLBACKS_HPP
#define CALLBACK_HPP

#include <string>

#include "core/Manager.hpp"

void addCourseCallback(Manager& manager, const std::string& code);
void removeCourseCallback(Manager& manager, const std::string& code);
void listCoursesCallback(Manager& manager);

void addAssessmentCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& weightInput, const std::string& gradeInput);
void removeAssessmentCallback(Manager& manager, const std::string& code, const std::string& name);

void updateGradeCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& gradeInput);
void showGradeCallback(Manager& manager, bool showScore, bool showExact, const std::string& code, const std::string& name);

void dumpCallback(Manager& manager);
void cleanCallback(Manager& manager);

#endif // !CALLBACKS_HPP
