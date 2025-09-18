#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Time;
class SceneManager;
class Input;

struct EngineContext
{
    sf::RenderWindow* window = nullptr; // - access bc can be needed in scenes
    SceneManager* sceneManager = nullptr; // - change petitions
    sf::Vector2u windowSize {1280, 720};
    Input* input{};
    Time* time = nullptr;
};