
#include "MouseState.h"

void MouseState::BeginFrame()
{
    _wheel = 0.f;
    _delta = {0,0};
}

void MouseState::Consume(const sf::Event& ev)
{
    if (const auto m = ev.getIf<sf::Event::MouseButtonPressed>())
    {
        _now.set(static_cast<size_t>(m->button), true);
    }
    else if (const auto m = ev.getIf<sf::Event::MouseButtonReleased>())
    {
        _now.set(static_cast<size_t>(m->button), false);
    }
    else if (const auto m = ev.getIf<sf::Event::MouseMoved>())
    {
        _position = m->position;
        _delta = _position - _prevPosition;
    }
    else if (const auto m = ev.getIf<sf::Event::MouseWheelScrolled>())
    {
        _wheel += m->delta;
    }
    else if (const auto m = ev.getIf<sf::Event::FocusLost>())
    {
        _now.reset();
    }
}

void MouseState::EndFrame()
{
    _prev = _now;
    _prevPosition = _position;
}

bool MouseState::GetButton(sf::Mouse::Button button) const
{
    return _now.test(static_cast<size_t>(button));
}

bool MouseState::GetButtonDown(sf::Mouse::Button button) const
{
    const auto i = static_cast<size_t>(button);
    return _now.test(i) && !_prev.test(i);
}

bool MouseState::GetButtonUp(sf::Mouse::Button button) const
{
    const auto i = static_cast<size_t>(button);
    return !_now.test(i) && _prev.test(i);
}

sf::Vector2i MouseState::GetMousePosition() const { return _position; }

sf::Vector2i MouseState::GetDeltaMousePosition() const { return _delta; }

float MouseState::GetWheelPosition() const { return _wheel; }
