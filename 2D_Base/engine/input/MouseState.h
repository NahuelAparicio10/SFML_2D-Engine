#pragma once
#include <bitset>

#include "SFML/Window/Event.hpp"

class MouseState
{
public:
    void BeginFrame();
    void Consume(const sf::Event& ev);
    void EndFrame();

    bool GetButton(sf::Mouse::Button button) const;
    bool GetButtonDown(sf::Mouse::Button button) const;
    bool GetButtonUp(sf::Mouse::Button button) const;

    sf::Vector2i GetMousePosition() const;
    sf::Vector2i GetDeltaMousePosition() const;
    float GetWheelDelta() const;

private:
    static constexpr size_t _btnCount = (size_t)sf::Mouse::ButtonCount;
    std::bitset<_btnCount> _now{}, _prev{};
    sf::Vector2i _position {0,0}, _prevPosition {0,0}, _delta {0,0};
    float _wheel = 0.f;
};


