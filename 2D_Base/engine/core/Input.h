#pragma once
#include "SFML/Window/Event.hpp"


class Input
{
public:

    void BeginFrame(); // Clears Deltas (wheel, text)m snapshot prev
    void Consume(const sf::Event& e);
    void EndFrame(); // Swap prev/current

    // Keyboard

    bool GetKey(sf::Keyboard::Key k) const; // held
    bool GetKeyDown(sf::Keyboard::Key k) const; // pressed this frame
    bool GetKeyUp(sf::Keyboard::Key k) const; // released this frame

    // Mouse

    bool GetMouseButton(sf::Mouse::Button b) const;
    bool GetMouseButtonDown(sf::Mouse::Button b) const;
    bool GetMouseButtonUp(sf::Mouse::Button b) const;

    void ClearAll();

    // Map actions (Unity - style)

    // Getters
    sf::Vector2i GetMousePosition() const;
    sf::Vector2i GetMouseDelta() const;
    float GetWheelDelta() const;
    const std::u32string& GetTextEntered() const;


private:

    sf::Vector2i _mousePosition {0,0};
    sf::Vector2i _mousePrevPosition {0,0};
    sf::Vector2i _mouseDelta {0,0};
    float _wheelDelta = 0.0f;

    std::u32string _textEntered;

};


