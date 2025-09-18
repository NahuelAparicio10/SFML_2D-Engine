#pragma once
#include "GamepadState.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "TextInput.h"
#include "SFML/Window/Event.hpp"


class Input
{
public:
    void BeginFrame(); // Clears Deltas (wheel, text)m snapshot prev
    void Consume(const sf::Event& e);
    void EndFrame(); // Swap prev/current

    const KeyboardState& GetKeyboard() const;
    const MouseState& GetMouse() const;
    const TextInput& GetTextInput() const;
    const GamepadState& GetGamepad() const;
    // Map actions (Unity - style)

private:
    KeyboardState _keyboardState;
    MouseState _mouseState;
    TextInput _textInput;
    GamepadState _gamepadState;
};


