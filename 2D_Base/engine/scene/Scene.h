#pragma once
#include "GameObject.h"

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

    std::vector<std::unique_ptr<GameObject>> gameobjects;
};


