#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <cmath>
#include <optional>

#include "SFML/Window/Joystick.hpp"
#include "Input.h"

// Translate hardware into intentions (Unity-like)
// Actions like GetAction, GetActionDown
class ActionMap
{
public:
    explicit ActionMap(const Input& input) : _in(&input) {}

    #pragma region Action Bindings (digitals)
    void AddKeyBindingForAction(std::string action, sf::Keyboard::Scancode key)
    {
        _actions[std::move(action)].keys.push_back(key);
    }
    void AddMouseBindingForAction(std::string action, sf::Mouse::Button btn)
    {
        _actions[std::move(action)].mouseButtons.push_back(btn);
    }
    void AddPadBindingForAction(std::string action, unsigned id, unsigned button)
    {
        _actions[std::move(action)].padButtons.push_back(PadButton{id, button});
    }
    void ClearAction(std::string_view action) {
        _actions.erase(std::string(action));
    }
    #pragma endregion

    #pragma region Axis Bindings (analog)
    // Keys: positive keys define (+1) and negatives (-1)
    void SetAxisKeys(std::string axis, std::vector<sf::Keyboard::Scancode> positive, std::vector<sf::Keyboard::Scancode> negative)
    {
        auto& x = _axes[std::move(axis)];
        x.keys.positive = std::move(positive);
        x.keys.negative = std::move(negative);
    }
    // Controller: analogic axis with options
    void SetAxisPad(std::string axis, unsigned id, sf::Joystick::Axis padAxis, bool invert = false, float weight = 1.f, float deadzone = 0.15f)
    {
        _axes[std::move(axis)].pad = PadAxis{id, padAxis, invert, weight, std::clamp(deadzone, 0.f, 0.99f)};
    }
    void ClearAxisPad(std::string_view axis)
    {
        if (auto it = _axes.find(std::string(axis)); it != _axes.end()) it->second.pad.reset();
    }
    void ClearAxis(std::string_view axis)
    {
        _axes.erase(std::string(axis));
    }
    #pragma endregion

    #pragma region Unity-like queries
    bool  GetAction(std::string_view action) const
    {
        if (const auto it = _actions.find(std::string(action)); it != _actions.end())
        {
            return AnyKeyHeld(it->second) || AnyMouseHeld(it->second) || AnyPadHeld(it->second);
        }
        return false;
    }
    bool  GetActionDown(std::string_view action) const
    {
        if (const auto it = _actions.find(std::string(action)); it != _actions.end())
        {
            return AnyKeyDown(it->second) || AnyMouseDown(it->second) || AnyPadDown(it->second);
        }
        return false;
    }
    bool  GetActionUp(std::string_view action) const
    {
        if (const auto it = _actions.find(std::string(action)); it != _actions.end())
        {
            return AnyKeyUp(it->second) || AnyMouseUp(it->second) || AnyPadUp(it->second);
        }
        return false;
    }

    // Value in [-1,1] by combining (discrete) keys and (analog) control
    float GetAxis(std::string_view axis) const
    {
        float v = 0.f;
        const auto it = _axes.find(std::string(axis));
        if (it == _axes.end()) return 0.f;

        // Keys
        if (AnyKeyPressed(it->second.keys.positive)) v += 1.f;
        if (AnyKeyPressed(it->second.keys.negative)) v -= 1.f;

        // Controller
        if (it->second.pad.has_value())
        {
            const auto& p = it->second.pad.value();
            float a = _in->GetGamepad().GetAxis(p.id, p.axis);
            // Deadzone additional for axis
            if (std::abs(a) < p.deadzone) a = 0.f;
            if (p.invert) a = -a;
            v += a * p.weight;
        }

        return std::clamp(v, -1.f, 1.f);
    }
    #pragma endregion
private:

    struct PadButton { unsigned id = 0, button = 0; };

    struct DigitalBinding
    {
        std::vector<sf::Keyboard::Scancode> keys;
        std::vector<sf::Mouse::Button>      mouseButtons;
        std::vector<PadButton>              padButtons;
    };

    struct AxisKeys
    {
        std::vector<sf::Keyboard::Scancode> positive;
        std::vector<sf::Keyboard::Scancode> negative;
    };

    struct PadAxis
    {
        unsigned id = 0;
        sf::Joystick::Axis axis = sf::Joystick::Axis::X;
        bool  invert = false;
        float weight = 1.f;
        float deadzone = 0.15f;
    };

    struct AxisBinding
    {
        AxisKeys keys;
        std::optional<PadAxis> pad;
    };

    const Input* _in;
    std::unordered_map<std::string, DigitalBinding> _actions;
    std::unordered_map<std::string, AxisBinding>    _axes;

    #pragma region Digital Helpers

    bool AnyKeyHeld(const DigitalBinding& b) const
    {
        for (auto k : b.keys)
            if (_in->GetKeyboard().GetKey(k))
            {
                return true;
            }
        return false;
    }
    bool AnyKeyDown(const DigitalBinding& b) const
    {
        for (auto k : b.keys)
            if (_in->GetKeyboard().GetKeyDown(k))
            {
                return true;
            }
        return false;
    }
    bool AnyKeyUp(const DigitalBinding& b) const
    {
        for (auto k : b.keys)
            if (_in->GetKeyboard().GetKeyUp(k))
            {
                return true;
            }
        return false;
    }
    bool AnyMouseHeld(const DigitalBinding& b) const
    {
        for (auto m : b.mouseButtons)
            if (_in->GetMouse().GetButton(m))
            {
                return true;
            }
        return false;
    }
    bool AnyMouseDown(const DigitalBinding& b) const
    {
        for (auto m : b.mouseButtons)
            if (_in->GetMouse().GetButtonDown(m))
            {
                return true;
            }
        return false;
    }
    bool AnyMouseUp(const DigitalBinding& b) const
    {
        for (auto m : b.mouseButtons)
            if (_in->GetMouse().GetButtonUp(m))
            {
                return true;
            }
        return false;
    }
    bool AnyPadHeld(const DigitalBinding& b) const
    {
        for (auto p : b.padButtons)
            if (_in->GetGamepad().GetButton(p.id, p.button))
            {
                return true;
            }
        return false;
    }
    bool AnyPadDown(const DigitalBinding& b) const
    {
        for (auto p : b.padButtons)
            if (_in->GetGamepad().GetButtonDown(p.id, p.button))
            {
                return true;
            }
        return false;
    }
    bool AnyPadUp(const DigitalBinding& b) const
    {
        for (auto p : b.padButtons)
            if (_in->GetGamepad().GetButtonUp(p.id, p.button))
            {
                return true;
            }
        return false;
    }
    bool AnyKeyPressed(const std::vector<sf::Keyboard::Scancode>& vv) const
    {
        for (auto k : vv)
            if (_in->GetKeyboard().GetKey(k))
            {
                return true;
            }
        return false;
    }
    #pragma endregion
};