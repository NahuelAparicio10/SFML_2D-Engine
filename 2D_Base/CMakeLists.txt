cmake_minimum_required(VERSION 4.0)
project(2D_Base)

include(FetchContent)
set(BUILD_SHARED_LIBS  OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)



set(CMAKE_CXX_STANDARD 20)

add_executable(2D_Base main.cpp
        engine/app/Application.cpp
        engine/app/Application.h
        engine/core/AppConfig.h
        engine/core/EngineContext.h
        engine/scene/Scene.h
        engine/scene/SceneManager.cpp
        engine/scene/SceneManager.h
        engine/core/Time.cpp
        engine/core/Time.h
        engine/input/Input.cpp
        engine/input/Input.h
        game/input/InputEnums.h
        engine/input/KeyboardState.cpp
        engine/input/KeyboardState.h
        engine/input/MouseState.cpp
        engine/input/MouseState.h
        engine/input/ActionMap.h
        engine/input/TextInput.cpp
        engine/input/TextInput.h
        engine/input/TextInput.cpp
        engine/input/Bindings.h
)
target_link_libraries(2D_Base sfml-graphics)