#include "KeyboardState.h"

void KeyboardState::BeginFrame() {}

void KeyboardState::Consume(const sf::Event& ev)
{
    if (auto key = ev.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode != sf::Keyboard::Scancode::Unknown)
            _now.set(static_cast<size_t>(key->scancode), true);
    }
    else if (auto key = ev.getIf<sf::Event::KeyReleased>())
    {
        if (key->scancode != sf::Keyboard::Scancode::Unknown)
            _now.set(static_cast<size_t>(key->scancode), false);
    }
    else if (ev.is<sf::Event::FocusLost>())
    {
        _now.reset();
    }
}

void KeyboardState::EndFrame()
{
    _prev = _now;
}

bool KeyboardState::GetKey(sf::Keyboard::Scancode s) const
{
    return _now.test(static_cast<size_t>(s));
}

bool KeyboardState::GetKeyDown(sf::Keyboard::Scancode s) const
{
    const std::size_t i = static_cast<std::size_t>(s);
    return _now.test(i) && !_prev.test(i);
}

bool KeyboardState::GetKeyUp(sf::Keyboard::Scancode s) const
{
    const std::size_t i = static_cast<std::size_t>(s);
    return !_now.test(i) && _prev.test(i);
}
