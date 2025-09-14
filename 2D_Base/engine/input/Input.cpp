#include "Input.h"

void Input::BeginFrame()
{
    _keyboardState.BeginFrame();
    _mouseState.BeginFrame();
    _textInput.BeginFrame();
}

void Input::Consume(const sf::Event& e)
{
    _keyboardState.Consume(e);
    _mouseState.Consume(e);
    _textInput.Consume(e);
}

void Input::EndFrame()
{
    _keyboardState.EndFrame();
    _mouseState.EndFrame();
    _textInput.EndFrame();
}

const KeyboardState& Input::GetKeyboard() const { return _keyboardState; }

const MouseState& Input::GetMouse() const { return _mouseState; }

const TextInput& Input::GetTextInput() const { return _textInput; }

