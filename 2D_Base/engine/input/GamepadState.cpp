#include "GamepadState.h"
#include <algorithm>

void GamepadState::BeginFrame()
{

}

void GamepadState::Consume(const sf::Event& ev)
{
    if (const auto j = ev.getIf<sf::Event::JoystickConnected>())
    {
        const unsigned id = j->joystickId;
        if (id < MaxJoysticks)
        {
            _pads[id].connected = true;

            // Reads initial status
            for (unsigned b = 0; b < MaxButtons; ++b)
            {
                bool pressed = sf::Joystick::isButtonPressed(id, b);
                _pads[id].now.set(b, pressed);
                _pads[id].prev.set(b, pressed);
            }

            for (std::size_t ai = 0; ai < AxisCount; ++ai)
            {
                auto ax = static_cast<sf::Joystick::Axis>(ai);
                float v = sf::Joystick::hasAxis(id, ax) ? Normalize100(sf::Joystick::getAxisPosition(id, ax)) : 0.f;
                _pads[id].axes[ai] = ApplyDeadZone(v);
            }
        }
    }
    // - If pad gets disconnect reset
    else if (const auto j = ev.getIf<sf::Event::JoystickDisconnected>())
    {
        ResetPad(j->joystickId);
    }
    else if (const auto j = ev.getIf<sf::Event::JoystickButtonPressed>())
    {
        const unsigned id = j->joystickId;
        const unsigned btn = j->button;

        if (id < MaxJoysticks && btn < MaxButtons)
        {
            _pads[id].connected = true;
            _pads[id].now.set(btn, true);
        }
    }
    else if (const auto j = ev.getIf<sf::Event::JoystickButtonReleased>())
    {
        const unsigned id = j->joystickId;
        const unsigned btn = j->button;

        if (id < MaxJoysticks && btn < MaxButtons)
        {
            _pads[id].now.set(btn, false);
        }
    }
    else if (const auto j = ev.getIf<sf::Event::JoystickMoved>())
    {
        const unsigned id = j->joystickId;

        if (id < MaxJoysticks)
        {
            const auto idx = AxisIndex(j->axis);

            if (idx < AxisCount)
            {
                _pads[id].axes[idx] = ApplyDeadZone(Normalize100(j->position));
            }
        }
    }

}

void GamepadState::EndFrame()
{
    for (auto& p : _pads)
    {
        p.prev = p.now;
    }
}

bool GamepadState::IsConnected(unsigned int id) const
{
    if (id >= MaxJoysticks) return false;
    return _pads[id].connected;
}

bool GamepadState::GetButton(unsigned int id, unsigned int button) const
{
    if (id >= MaxJoysticks || button >= MaxButtons) return false;

    return _pads[id].now.test(button);
}

bool GamepadState::GetButtonDown(unsigned int id, unsigned int button) const
{
    if (id >= MaxJoysticks || button >= MaxButtons) return false;

    const auto& p = _pads[id];
    return p.now.test(button) && !p.prev.test(button);
}

bool GamepadState::GetButtonUp(unsigned int id, unsigned int button) const
{
    if (id >= MaxJoysticks || button >= MaxButtons) return false;

    const auto& p = _pads[id];
    return !p.now.test(button) && p.prev.test(button);

}

float GamepadState::GetAxis(unsigned int id, sf::Joystick::Axis axis) const
{
    if (id >= MaxJoysticks) return 0.f;


    const auto& p = _pads[id];
    const auto idx = AxisIndex(axis);

    if (idx >= AxisCount) return 0.f;

    return p.axes[idx];
}

void GamepadState::SetDeadzone(float dz)
{
    _deadzone = std::clamp(dz, 0.0f, 0.99f);
}

float GamepadState::GetDeadzone() const { return _deadzone; }

std::size_t GamepadState::AxisIndex(sf::Joystick::Axis ax) { return static_cast<std::size_t>(ax); }

float GamepadState::Normalize100(float v) { return std::clamp(v / 100.f, -1.f, 1.f); }

float GamepadState::ApplyDeadZone(float v) const
{
    float a = std::abs(v);

    if (a < _deadzone) return 0.f;
    float sign = (v < 0.f) ? -1.f : 1.f;

    return sign * ((a - _deadzone) / (1.f - _deadzone));
}

void GamepadState::ResetPad(unsigned int id)
{
    if (id >= MaxJoysticks) return;

    auto& pad = _pads[id];

    pad.connected = false;
    pad.now.reset();
    pad.prev.reset();
    pad.axes.fill(0.f);
}
