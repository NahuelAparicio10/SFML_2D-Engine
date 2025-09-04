#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include <SFML/Window/Event.hpp>

class Scene;
struct EngineContext;

class SceneManager
{
public:
    enum class TransitionType : std::uint8_t { Replace, Clear };
    using ScenePtr = std::unique_ptr<Scene>;

    // Petitions (applied in processPending)
    void RequestReplace(ScenePtr scene);
    void RequestClear();

    // Applies the queue (called once per frame before update)
    void ProcessPending(EngineContext& ctx);

    // Loop
    void HandleEvent(EngineContext& ctx, const sf::Event& ev);
    void FixedUpdate(EngineContext& ctx, float dt);
    void Update(EngineContext& ctx, float dt);
    void Render(EngineContext& ctx);

    // Status
    Scene* GetCurrent() const;
    bool IsEmpty() const;
private:

    struct Transition
    {
        TransitionType type;
        ScenePtr scene; // Only for replace
    };

    // Double command buffer so we don't interfer while processing
    std::vector<Transition> _pending, _processing;
    ScenePtr _currentScene;
};


