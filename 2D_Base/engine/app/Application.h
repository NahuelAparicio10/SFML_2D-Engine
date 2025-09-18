#pragma once
#include "../core/AppConfig.h"
#include "../core/EngineContext.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../scene/SceneManager.h"
#include "../core/Time.h"
#include "../input/Input.h"

class Application
{
public:
    explicit Application(const AppConfig & config);
    ~Application();

    void Run();

private:
    void HandleEvents();
    void Render();
    void FixedUpdate(double dt);
    void Update(float dt);

private:
    AppConfig _config;
    sf::RenderWindow _window;
    EngineContext _context;
    SceneManager _sceneManager;
    Time _time;
    Input _input;
    sf::Clock _clock;
};

