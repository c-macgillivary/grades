#ifndef CALLBACKS_HPP
#define CALLBACK_HPP

#include <string>

#include "core/Manager.hpp"

void addCourseCallback(Manager& manager, const std::string& code);
void removeCourseCallback(Manager& manager, const std::string& code);
void listCoursesCallback(Manager& manager);

void addAssessmentCallback(Manager& manager, const std::string& code, const std::string& name, const std::string& weight, const std::string& grade);
void removeAssessmentCallback(Manager& manager, const std::string& code, const std::string& name);

void updateGradeCallback(Manager& manager, const std::string& code, const std::string& name, std::string grade);
void showGradeCallback(Manager& manager, bool showScore, bool showPercent, bool showExact, const std::string& code, const std::string& name);

void dumpCallback(Manager& manager);
void cleanCallback(Manager& manager);

#endif // !CALLBACKS_HPP
