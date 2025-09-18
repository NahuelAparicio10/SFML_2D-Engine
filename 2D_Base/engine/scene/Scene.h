#pragma once
#include "SFML/Window/Event.hpp"

struct EngineContext;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void OnStart(EngineContext&){}
    virtual void OnStop(EngineContext&) {}
    virtual void HandleEvent(EngineContext&, const sf::Event&) {}
    virtual void FixedUpdate(EngineContext&, double) = 0;
    virtual void Update(EngineContext&, float) = 0;
    virtual void Render(EngineContext&) {}
};


