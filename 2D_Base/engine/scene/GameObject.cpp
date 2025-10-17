#include "GameObject.h"
#include "../components/Transform.h"
#include <algorithm>

GameObject::GameObject()
{
    transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
    for (int i = static_cast<int>(_components.size()) - 1; i >= 0; --i)
    {
        if (_components[i])
        {
            _components[i]->OnDestroy();
            delete _components[i];
            _components[i] = nullptr;
        }
    }

    _components.clear();

    // Detach from father
    if (_parent)
    {
        auto& s = _parent->_children;
        s.erase(std::remove(s.begin(), s.end(), this), s.end());
        _parent = nullptr;
    }
}

int GameObject::GetID() const { return _id; }

std::string GameObject::GetTag() { return _tag; }

void GameObject::SetID(const int id) { _id = id; }

void GameObject::SetTag(const std::string& tag) { _tag = tag; }

GameObject* GameObject::GetParent() const { return _parent; }

std::size_t GameObject::ChildCount() const { return _children.size(); }

GameObject* GameObject::GetChild(std::size_t i) const { return _children[i]; }

void GameObject::SetActive(bool v)
{
    if (_activeSelf == v) return;

    _activeSelf = v;

    bool desired = _parent ? (_parent->_activeInHierarchy && _activeSelf) : _activeSelf;

    InternalSetActiveInHierarchy(desired);
}

bool GameObject::ActiveSelf() const{ return _activeSelf; }

bool GameObject::IsActiveInHierarchy() const { return _activeInHierarchy; }

void GameObject::InternalSetParent(GameObject* newParent)
{
    if (_parent == newParent) return;

    if (_parent)
    {
        auto& s = _parent->_children;
        s.erase(std::remove(s.begin(), s.end(), this), s.end());
    }

    _parent = newParent;

    _parent->_children.push_back(this);

    bool desired = _parent ? (_parent->_activeInHierarchy && _activeSelf) : _activeSelf;
    InternalSetActiveInHierarchy(desired);
}

void GameObject::InternalSetActiveInHierarchy(bool desired)
{
    if (_activeInHierarchy == desired) return;

    InvokeEnableDisableForAll(desired);
    _activeInHierarchy = desired;

    for (auto* children : _children)
    {
        children->InternalSetActiveInHierarchy(desired && _activeSelf);
    }
}

void GameObject::InvokeEnableDisableForAll(bool becomingActive)
{
    for (auto* component : _components)
    {
        if (!component->enabled) continue;

        if (becomingActive)
        {
            if (!component->enabledInvoked)
            {
                component->OnEnable();
                component->enabledInvoked = true;
            }
        }
        else
        {
            if (!component->enabledInvoked)
            {
                component->OnDisable();
                component->enabledInvoked = false;
            }
        }
    }
}

void GameObject::InternalCallStartIfNeeded()
{
    if (!_activeInHierarchy) return;

    for (auto* component : _components)
    {
        if (!component->enabled || component->started) continue;
        component->Start();
        component->started = true;
    }
}

void GameObject::InternalFixedUpdate(double dt)
{
    if (!_activeInHierarchy) return;


    for (auto* c : _components)
    {
        if (!c || !c->enabled) continue;
        c->FixedUpdate(dt);
    }
}

void GameObject::InternalUpdate(float dt)
{
    if (!_activeInHierarchy) return;

    InternalCallStartIfNeeded();

    for (auto* c : _components)
    {
        if (!c || !c->enabled) continue;
        c->Update(dt);
    }
}

void GameObject::InternalLateUpdate(float dt)
{
    if (!_activeInHierarchy) return;

    for (auto* c : _components)
    {
        if (!c || !c->enabled) continue;
        c->LateUpdate(dt);
    }
}

void GameObject::InternalDestroy()
{

}







