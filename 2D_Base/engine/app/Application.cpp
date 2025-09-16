#include "Application.h"
#include "../input/Input.h"

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

    if (!_config.vsync && _config.fpsCap > 0)
    {
        _window.setFramerateLimit(_config.fpsCap);
    }

    _context.window = &_window;
    _context.sceneManager = &_sceneManager;
    _context.windowSize = _window.getSize();
    _context.time = &_time;
    _context.input = &_input;

    // Sets the fixed step from app configuration
    _time.SetFixedStep(config.fixedStep > 0.f ? config.fixedStep : (1.f/60.f));
}

Application::~Application() = default;

void Application::Run()
{
    //const float fixed = _config.fixedStep > 0.f ? _config.fixedStep : 1.f/60.f;

    while (_window.isOpen())
    {
        _input.BeginFrame();

        HandleEvents();

        // Applies requested transitions between scenes
        _sceneManager.ProcessPending(_context);

        //Advances frame time
        float rawDt = _clock.restart().asSeconds();
        _time.Tick(rawDt);

        // Avoidance of big time jumps (alt-tab, window drag, etc.)
        //frameDt = std::min(frameDt, 0.25f);


        while (_time.StepFixed())
        {
            FixedUpdate(_time.GetFixedStep());
        }

        Update(_time.GetFrameDt());

        Render();

        _input.EndFrame();

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
    while (auto event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
            continue;
        }
        if (const auto* r = event->getIf<sf::Event::Resized>())
        {
            _context.windowSize = r->size;
        }

        _input.Consume(*event);

        _sceneManager.HandleEvent(_context, *event);
    }
}

void Application::FixedUpdate(double dt)
{
    _sceneManager.FixedUpdate(_context, dt);
}

void Application::Update(float dt)
{
    _sceneManager.Update(_context, dt);
}
