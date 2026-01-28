#include "Scene.h"
#include <memory>

GameObject* Scene::Instantiate()
{
    // Safe: instantiated objects become active at the next safe point.
    auto go = std::make_unique<GameObject>();
    GameObject* raw = go.get();
    _pendingInstantiate.push_back(std::move(go));
    return raw;
}


void Scene::Destroy(GameObject* go)
{
    if (!go) return;
    if (_pendingDestroySet.find(go) != _pendingDestroySet.end()) return;

    _pendingDestroy.push_back(go);
    _pendingDestroySet.insert(go);
}

void Scene::FixedUpdate(EngineContext& ctx, double dt)
{
    FlushInstantiateQueue();
    ProcessStartPending();

    OnFixedUpdate(ctx, dt);

    _iterating = true;
    for (auto& go : _gameObjects)
    {
        if (!go) continue;
        if (IsPendingDestroy(go.get())) continue;
        go->InternalFixedUpdate(dt);
    }
    _iterating = false;
}

void Scene::Update(EngineContext& ctx, float dt)
{
    FlushInstantiateQueue();
    ProcessStartPending();

    OnUpdate(ctx, dt);

    _iterating = true;
    for (auto& go : _gameObjects)
    {
        if (!go) continue;
        if (IsPendingDestroy(go.get())) continue;
        go->InternalUpdate(dt);
    }
    _iterating = false;
}

void Scene::LateUpdate(EngineContext& ctx, float dt)
{
    OnLateUpdate(ctx, dt);

    _iterating = true;
    for (auto& go : _gameObjects)
    {
        if (!go) continue;
        if (IsPendingDestroy(go.get())) continue;
        go->InternalLateUpdate(dt);
    }
    _iterating = false;

    ProcessDestroyPending();
}

void Scene::FlushInstantiateQueue()
{
    if (_pendingInstantiate.empty()) return;

    // Move pending into the live list. Start() will be handled by ProcessStartPending.
    for (auto& go : _pendingInstantiate)
    {
        _gameObjects.push_back(std::move(go));
    }
    _pendingInstantiate.clear();
}

void Scene::ProcessStartPending()
{
    // Start() is invoked on-demand per GameObject, and is idempotent.
    for (auto& go : _gameObjects)
    {
        if (!go) continue;
        if (IsPendingDestroy(go.get())) continue;
        go->InternalCallStartIfNeeded();
    }
}

void Scene::ProcessDestroyPending()
{
    if (_pendingDestroy.empty()) return;

    // Call InternalDestroy before releasing memory.
    for (auto* go : _pendingDestroy)
    {
        if (!go) continue;
        go->InternalDestroy();
    }

    _gameObjects.erase(
        std::remove_if(_gameObjects.begin(),_gameObjects.end(),
            [&](const std::unique_ptr<GameObject>& ptr)
            {
                return !ptr || _pendingDestroySet.find(ptr.get()) != _pendingDestroySet.end();
            }),_gameObjects.end());

    _pendingDestroy.clear();
    _pendingDestroySet.clear();
}

bool Scene::IsPendingDestroy(const GameObject* go) const
{
    return go && (_pendingDestroySet.find(go) != _pendingDestroySet.end());
}