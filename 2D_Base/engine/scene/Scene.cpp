#include "Scene.h"
#include <memory>

GameObject* Scene::Instantiate()
{
    // Safe instantiated objects become active at the next safe point.
    auto go = std::make_unique<GameObject>();
    GameObject* raw = go.get();
    _pendingInstantiate.push_back(std::move(go));
    return raw;
}


void Scene::Destroy(GameObject* go)
{
    if (!go) return;

    // If the object was instantiated this frame and never entered the live list,
    // destroy it immediately by removing it from the instantiate queue.
    for (auto it = _pendingInstantiate.begin(); it != _pendingInstantiate.end(); ++it)
    {
        if (it->get() == go)
        {
            go->InternalDestroy();
            _pendingInstantiate.erase(it);
            return;
        }
    }

    if (_pendingDestroySet.contains(go)) return;

    _pendingDestroy.push_back(go);
    _pendingDestroySet.insert(go);
}

void Scene::DestroyHierarchy(GameObject* root)
{
    if (!root) return;

    // Collect the hierarchy first (preorder), then enqueue destruction in reverse order (children -> parent).
    std::vector<GameObject*> stack;
    std::vector<GameObject*> collected;
    collected.reserve(32);

    std::unordered_set<const GameObject*> visited;
    visited.reserve(64);

    stack.push_back(root);

    while (!stack.empty())
    {
        GameObject* node = stack.back();
        stack.pop_back();

        if (!node) continue;
        if (visited.contains(node)) continue;
        visited.insert(node);

        collected.push_back(node);

        // we do NOT destroy anything while collecting, so hierarchy pointers are stable here.
        const std::size_t count = node->ChildCount();
        for (std::size_t i = 0; i < count; ++i)
        {
            if (GameObject* child = node->GetChild(i))
            {
                stack.push_back(child);
            }
        }
    }

    // Destroy children first, then parent (reverse)
    for (int i = static_cast<int>(collected.size()) - 1; i >= 0; --i)
    {
        EnqueueDestroySingle(collected[i]);
    }
}

void Scene::EnqueueDestroySingle(GameObject* go)
{
    if (!go) return;

    // If the object was instantiated this frame and never entered the live list,
    // destroy it immediately by removing it from the instantiate queue.
    for (auto it = _pendingInstantiate.begin(); it != _pendingInstantiate.end(); ++it)
    {
        if (it->get() == go)
        {
            go->InternalDestroy();
            _pendingInstantiate.erase(it);
            return;
        }
    }

    if (_pendingDestroySet.contains(go)) return;

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
    // Safe point between Update and LateUpdate.
    FlushInstantiateQueue();
    ProcessStartPending();

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
    assert(!_iterating && "FlushInstantiateQueue must not run while iterating _gameObjects");
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
    assert(!_iterating && "ProcessDestroyPending must not run while iterating _gameObjects");
    if (_pendingDestroy.empty()) return;

    // Call InternalDestroy before releasing memory.
    for (auto* go : _pendingDestroy)
    {
        if (!go) continue;
        go->InternalDestroy();
    }

    std::erase_if(_gameObjects,
        [&](const std::unique_ptr<GameObject>& ptr)
        {
            return !ptr || _pendingDestroySet.contains(ptr.get());
        });

    _pendingDestroy.clear();
    _pendingDestroySet.clear();
}

bool Scene::IsPendingDestroy(const GameObject* go) const
{
    return go && (_pendingDestroySet.contains(go));
}