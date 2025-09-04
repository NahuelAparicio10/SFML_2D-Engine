#include "Application.h"
#include <algorithm>

Application::Application(const AppConfig & config) : _config(config)
{
    sf::ContextSettings contextSetting;
    contextSetting.antiAliasingLevel = 0;

    _window.create(
        sf::VideoMode({_config.width, _config.height}),
        _config.appName,
        sf::Style::Default,
        sf::State::Windowed,
        contextSetting);

    _window.setVerticalSyncEnabled(_config.vsync);
    if (!_config.vsync && _config.fpsCap > 0) {
        _window.setFramerateLimit(_config.fpsCap);
    }

    _context.window = &_window;
    _context.sceneManager = &_sceneManager;
    _context.windowSize = _window.getSize();
}

Application::~Application() = default;

void Application::Run()
{
    const float fixed = _config.fixedStep > 0.f ? _config.fixedStep : 1.f/60.f;

    while (_window.isOpen())
    {
        HandleEvents();

        // Applies requested transitions between scenes
        _sceneManager.ProcessPending(_context);

        float frameDt = _clock.restart().asSeconds();

        // Avoidance of big time jumps (alt-tab, window drag, etc)
        frameDt = std::min(frameDt, 0.25f);

        _accumulator += frameDt;

        while (_accumulator >= fixed)
        {
            FixedUpdate(fixed);
            _accumulator -= fixed;
        }

        Update(frameDt);

        Render();
    }
}

void Application::Render()
{
    _window.clear(sf::Color::Black);

    _sceneManager.Render(_context);

    _window.display();
}

void Application::HandleEvents()
{
    while (const std::optional event = _window.pollEvent())
    {
        _sceneManager.HandleEvent(_context, *event);
    }
}

void Application::FixedUpdate(float dt)
{
    _sceneManager.FixedUpdate(_context, dt);
}

void Application::Update(float dt)
{
    _sceneManager.Update(_context, dt);
}
