#pragma once
#include "SFML/Window/Event.hpp"

struct EngineContext;

class Scene
{
public:
    virtual ~Scene();

    virtual void OnStart(EngineContext& ctx);
    virtual void OnStop(EngineContext& ctx);

    virtual void HandleEvent(EngineContext& ctx, const sf::Event& ev);
    virtual void FixedUpdate(EngineContext& ctx, float dt) = 0;
    virtual void Update(EngineContext& ctx, float dt) = 0;
    void Render(EngineContext& ctx);
protected:
    //Game Objects
    //Canvas?
};


