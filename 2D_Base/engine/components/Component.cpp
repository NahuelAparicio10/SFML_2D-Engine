#include "Component.h"

#include "../scene/GameObject.h"

void Component::SetEnabled(bool v)
{
    if (enabled == v) return;

    enabled = v;

    if (!gameObject) return;

    if (gameObject->IsActiveInHierarchy())
    {
        if (enabled)
        {
            if (!enabledInvoked)
            {
                OnEnable();
                enabledInvoked = true;
            }
        }
        else
        {
            if (enabledInvoked)
            {
                OnDisable();
                enabledInvoked = false;
            }
        }
    }
}
