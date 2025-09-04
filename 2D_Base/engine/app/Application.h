#pragma once
#include "../core/AppConfig.h"
#include "../core/EngineContext.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../scene/SceneManager.h"

class Application
{
public:
    explicit Application(const AppConfig & config);
    ~Application();

    void Run();

private:
    void HandleEvents();
    void Render();
    void FixedUpdate(float dt);
    void Update(float dt);

private:
    AppConfig _config;
    sf::RenderWindow _window;
    EngineContext _context;
    SceneManager _sceneManager;
    sf::Clock _clock;
    float _accumulator = 0.f;
};

