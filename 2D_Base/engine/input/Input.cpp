#include "Input.h"

void Input::BeginFrame()
{
    _keyboardState.BeginFrame();
    _mouseState.BeginFrame();
    _textInput.BeginFrame();
    _gamepadState.BeginFrame();
}

void Input::Consume(const sf::Event& e)
{
    _keyboardState.Consume(e);
    _mouseState.Consume(e);
    _textInput.Consume(e);
    _gamepadState.Consume(e);
}

void Input::EndFrame()
{
    _keyboardState.EndFrame();
    _mouseState.EndFrame();
    _textInput.EndFrame();
    _gamepadState.EndFrame();
}

const KeyboardState& Input::GetKeyboard() const { return _keyboardState; }

const MouseState& Input::GetMouse() const { return _mouseState; }

const TextInput& Input::GetTextInput() const { return _textInput; }

const GamepadState& Input::GetGamepad() const { return _gamepadState; }

