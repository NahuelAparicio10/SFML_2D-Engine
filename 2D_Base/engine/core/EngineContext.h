#pragma once
#include <SFML/Graphics.hpp>

class SceneManager;

struct EngineContext
{
    sf::RenderWindow* window = nullptr; // - acces bc can be needed in scenes
    SceneManager* sceneManager = nullptr; // - change petitions
    sf::Vector2u windowSize;
};