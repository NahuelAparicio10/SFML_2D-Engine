#pragma once
#include <string>

#include "../components/Component.h"
#include <vector>

class Transform;

class GameObject
{
public:
    GameObject();
    ~GameObject();

    // Identity and tag
    int GetID() const;
    void SetID(int id);
    std::string GetTag();
    void SetTag(const std::string& tag);

    // Hierachy
    GameObject* GetParent() const;
    std::size_t ChildCount() const;
    GameObject* GetChild(std::size_t i) const;
    void InternalSetParent(GameObject* newParent);


    void SetActive(bool v);
    bool ActiveSelf() const;
    bool IsActiveInHierarchy() const;

#pragma region Component Handling
    template <typename T, typename ...Args>
    T* AddComponent(Args && ...args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        T* c = new T(std::forward<Args>(args)...);
        c->gameObject = this;
        _components.push_back(c);

        c->Awake();

        // If we are already enabled and the component is enabled we throw OnEnable

        if (_activeInHierarchy && c->enabled && !c->enabledInvoked)
        {
            c->OnEnable();
            c->enabledInvoked = true;
        }
        return c;
    }

    template<typename T> T* GetComponent()
    {
        std::type_index typeIndex = typeid(T);

        for (auto& component : _components)
        {
            if (component && component->GetType() == typeIndex)
            {
                return static_cast<T*>(component);
            }
        }
        return nullptr;
    }

    template<typename T> T* GetOrAdd()
    {
        std::type_index typeIndex = typeid(T);

        for (auto& component : _components)
        {
            if (component && component->GetType() == typeIndex)
            {
                return static_cast<T*>(component);
            }
        }

        AddComponent<T>();
        return static_cast<T*>(_components.back());
    }
#pragma endregion

    Transform* transform = nullptr;

    // Engine calls (frame order)
    void InternalCallStartIfNeeded();
    void InternalFixedUpdate(double dt);
    void InternalUpdate(float dt);
    void InternalLateUpdate(float dt);
    void InternalSetActiveInHierarchy(bool v);
    void InternalDestroy();

private:
    void InvokeEnableDisableForAll(bool becomingActive);

    int _id = 0;
    std::string _tag = "Default";
    bool _activeSelf = true;
    bool _activeInHierarchy = true;

    GameObject* _parent = nullptr;
    std::vector<GameObject*> _children;

    std::vector<Component*> _components;

};




