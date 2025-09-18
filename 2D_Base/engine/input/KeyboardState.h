#pragma once
#include <bitset>
#include "SFML/Window/Event.hpp"

class KeyboardState
{
public:
    void BeginFrame();
    void Consume(const sf::Event& ev);
    void EndFrame();

    bool GetKey(sf::Keyboard::Scancode s) const; // held
    bool GetKeyDown(sf::Keyboard::Scancode s) const; // pressed this frame
    bool GetKeyUp(sf::Keyboard::Scancode s) const; // released this frame

private:
    static constexpr size_t Count = (size_t)sf::Keyboard::ScancodeCount;
    std::bitset<Count> _now, _prev;

};


