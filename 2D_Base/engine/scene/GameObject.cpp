#include "GameObject.h"
#include "../components/Transform.h"
#include <algorithm>

GameObject::GameObject()
{
    transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
    // Ensure deterministic destruction even if the owner didn't call InternalDestroy().
    InternalDestroy();

    for (int i = static_cast<int>(_components.size()) - 1; i >= 0; --i)
    {
        delete _components[i];
        _components[i] = nullptr;
    }

    _components.clear();

    // Detach from father
    if (_parent)
    {
        auto& s = _parent->_children;
        std::erase(s, this);
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
        std::erase(s, this);
    }

    _parent = newParent;

    if (_parent)
    {
        _parent->_children.push_back(this);
    }

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
            if (component->enabledInvoked)
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
    if (_destroyInvoked) return;

    _destroyInvoked = true;

    // Detach children to avoid leaving them with a dangling parent pointer.
    // ! Warning ! this does NOT destroy children; it only prevents invalid hierarchy references.
    if (!_children.empty())
    {
        auto childrenCopy = _children;
        _children.clear();
        for (auto* child : childrenCopy)
        {
            if (!child) continue;
            child->_parent = nullptr;
            bool desired = child->_activeSelf;
            child->InternalSetActiveInHierarchy(desired);
        }
    }

    // Detach from parent immediately so the hierarchy stays consistent within the frame.
    if (_parent)
    {
        auto& s = _parent->_children;
        std::erase(s, this);
        _parent = nullptr;
    }

    // If we were active in hierarchy, disable enabled components first.
    if (_activeInHierarchy)
    {
        for (auto* c : _components)
        {
            if (!c || !c->enabled) continue;
            if (c->enabledInvoked)
            {
                c->OnDisable();
                c->enabledInvoked = false;
            }
        }
    }

    // OnDestroy must be invoked once, before memory is released.
    for (auto* c : _components)
    {
        if (!c) continue;
        c->OnDestroy();
    }

    _activeInHierarchy = false;
}







