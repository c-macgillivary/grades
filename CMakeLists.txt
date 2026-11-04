cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
get_filename_component(PROJECT_NAME_RAW ${PROJECT_ROOT_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME_CLEAN ${PROJECT_NAME_RAW})

project(${PROJECT_NAME_CLEAN})

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)

# NEW: FetchContent support
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

add_executable(${PROJECT_NAME_CLEAN} 
  src/main.cpp
  src/Class.cpp
  src/ClassManager.cpp
  src/Assessment.cpp
)

target_include_directories(${PROJECT_NAME_CLEAN}
  PRIVATE
  $<BUILD_INTERFACE:${PROJECT_ROOT_DIR}/include/>
)

target_link_libraries(${PROJECT_NAME_CLEAN}
  PRIVATE
  CLI11::CLI11
  nlohmann_json::nlohmann_json
)

install(TARGETS ${PROJECT_NAME_CLEAN} DESTINATION bin)
