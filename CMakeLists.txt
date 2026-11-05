cmake_minimum_required(VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(grades)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)

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

add_executable(grades
  src/core/main.cpp
  src/core/Class.cpp
  src/core/ClassManager.cpp
  src/core/Assessment.cpp
)

target_include_directories(grades
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries(grades
  PRIVATE
  CLI11::CLI11
  nlohmann_json::nlohmann_json
)

install(TARGETS grades DESTINATION bin)
