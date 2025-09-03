cmake_minimum_required(VERSION 4.0)
project(2D_Base)

include(FetchContent)
set(BUILD_SHARED_LIBS  OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)



set(CMAKE_CXX_STANDARD 20)

add_executable(2D_Base main.cpp
        GameManager.cpp
        GameManager.h)
target_link_libraries(2D_Base sfml-graphics)