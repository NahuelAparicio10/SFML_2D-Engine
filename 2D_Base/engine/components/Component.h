#pragma once
#include <typeindex>

class GameObject;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    GameObject* gameObject = nullptr;
    bool enabled = true;

    // Lifecycle state (engine-managed)
    bool started = false;
    bool enabledInvoked = false;

    virtual void Awake() {}
    virtual void OnEnable() {}
    virtual void Start() {}
    virtual void FixedUpdate(double) {}
    virtual void Update(float) {}
    virtual void LateUpdate(float) {}
    virtual void OnDisable() {}
    virtual void OnDestroy() {}

    void SetEnabled(bool v);

    virtual std::type_index GetType() const = 0;
};