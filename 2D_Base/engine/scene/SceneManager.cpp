#include "SceneManager.h"
#include "Scene.h"


void SceneManager::RequestReplace(ScenePtr scene)
{
    _pending.push_back({TransitionType::Replace, std::move(scene)});
}

void SceneManager::RequestClear()
{
    _pending.push_back({TransitionType::Clear, {}});
}

void SceneManager::ProcessPending(EngineContext& ctx)
{
    if (_pending.empty()) return;

    _processing.clear();
    _processing.swap(_pending);

    // If last is Clear then ignore previous commands
    if (!_processing.empty() && _processing.back().type == TransitionType::Clear)
    {
        _processing.clear();
        _processing.push_back({TransitionType::Clear, {}});
    }

    for (auto & [type, scene] : _processing)
    {
        switch (type)
        {
            case TransitionType::Replace:
                if (_currentScene) _currentScene->OnStop(ctx);
                _currentScene = std::move(scene);
                _currentScene->OnStart(ctx);
            break;

            case TransitionType::Clear:
                if (_currentScene)
                {
                    _currentScene->OnStop(ctx);
                    _currentScene.reset();
                }
            break;

            default:
                // Error - No transition type
            break;
        }
    }

    _processing.clear();
}

void SceneManager::HandleEvent(EngineContext& ctx, const sf::Event& ev)
{
    if (IsEmpty()) return;

    _currentScene->HandleEvent(ctx, ev);
}

void SceneManager::FixedUpdate(EngineContext& ctx, double dt)
{
    if (IsEmpty()) return;
    _currentScene->FixedUpdate(ctx, dt);
}

void SceneManager::Update(EngineContext& ctx, float dt)
{
    if (IsEmpty()) return;

    _currentScene->Update(ctx, dt);
}

void SceneManager::Render(EngineContext& ctx)
{
    if (IsEmpty()) return;

    _currentScene->Render(ctx);
}

Scene* SceneManager::GetCurrent() const { return _currentScene.get(); }

bool SceneManager::IsEmpty() const { return !_currentScene; }

