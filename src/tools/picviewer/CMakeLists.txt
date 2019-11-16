cmake_minimum_required(VERSION 3.5)

project(picviewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Have to do this after finding Qt5, I think that tries to use an older version
set(CMAKE_CXX_STANDARD 17)

add_executable(picviewer
  main.cpp
  Window.cpp
  Window.h
)

target_link_libraries(picviewer PRIVATE Qt5::Widgets genieutils)
