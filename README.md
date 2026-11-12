# Grades Tracker
[![Build](https://github.com/c-macgillivary/grades/actions/workflows/cmake-ubuntu-build.yaml/badge.svg)](https://github.com/c-macgillivary/grades/actions/workflows/cmake-ubuntu-build.yaml)
This is a project meant to track grades in a given course.
Functionality is being added daily.
Planned features include:
- Current mark calcuation
- Best of X assignments
- What grade do I need to get X final grade?
- Highest possible final grade 
- AND much more (seriously I have an ideas file with 40+ lines)

Saves to `$XDG_DATA_HOME/.local/share/grades/classes.json`. Uses `$HOME` if `$XDG_DATA_HOME` is not set.
