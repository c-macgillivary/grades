cmake_minimum_required(VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(grades)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)

# Fetch dependencies
include(FetchContent)

set(CLI11_PRECOMPILED ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.7.2
)
FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
  json
  URL
  https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_library(core STATIC
  src/core/Class.cpp
  src/core/ClassManager.cpp
  src/core/Assessment.cpp
)

target_link_libraries(core PUBLIC nlohmann_json::nlohmann_json)

target_include_directories(core PUBLIC include)

add_executable(grades
    src/cli/main.cpp
    src/cli/ClassCommands.cpp
)

target_link_libraries(grades
  PRIVATE
  core
  CLI11::CLI11
)

install(TARGETS grades DESTINATION bin)
