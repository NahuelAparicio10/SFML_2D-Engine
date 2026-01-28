#pragma once
#include <algorithm>
#include <memory>
#include <unordered_set>
#include "GameObject.h"
#include "SFML/Window/Event.hpp"

struct EngineContext;

class Scene
{
public:
    virtual ~Scene() = default;

    // Scene Life Cycle
    virtual void OnStart(EngineContext&){}
    virtual void OnStop(EngineContext&) {}
    virtual void HandleEvent(EngineContext&, const sf::Event&) {}
    virtual void Render(EngineContext&) {}

    // Engine calls (frame order)
    virtual void FixedUpdate(EngineContext&, double) final;
    virtual void Update(EngineContext&, float) final;
    virtual void LateUpdate(EngineContext&, float) final;

    // GameObject management
    GameObject* Instantiate();

    template <typename TComponent, typename... Args>
    TComponent* Instantiate(Args&&... args)
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must derive from Component");
        GameObject* go = Instantiate();
        return go->AddComponent<TComponent>(std::forward<Args>(args)...);
    }

    void Destroy(GameObject* go);

    // Read-only access for debugging/tools.
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return _gameObjects; }

protected:
    // Optional hooks for scene-specific logic.
    virtual void OnFixedUpdate(EngineContext&, double) {}
    virtual void OnUpdate(EngineContext&, float) {}
    virtual void OnLateUpdate(EngineContext&, float) {}

private:
    void FlushInstantiateQueue();
    void ProcessStartPending();
    void ProcessDestroyPending();
    bool IsPendingDestroy(const GameObject* go) const;

private:
    std::vector<std::unique_ptr<GameObject>> _gameObjects;

    // Safe queues (no create/destroy in the middle of iteration)
    std::vector<std::unique_ptr<GameObject>> _pendingInstantiate;
    std::vector<GameObject*> _pendingDestroy;
    std::unordered_set<const GameObject*> _pendingDestroySet;

    // Re-entrancy guard (debug safety)
    bool _iterating = false;
};
