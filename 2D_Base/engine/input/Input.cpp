#include "Input.h"
void Input::BeginFrame()
{

}

void Input::Consume(const sf::Event& e)
{
}

void Input::EndFrame()
{
}

bool Input::GetKey(sf::Keyboard::Key k) const
{
}

bool Input::GetKeyDown(sf::Keyboard::Key k) const
{
}

bool Input::GetKeyUp(sf::Keyboard::Key k) const
{
}

bool Input::GetMouseButton(sf::Mouse::Button b) const
{
}

bool Input::GetMouseButtonDown(sf::Mouse::Button b) const
{
}

bool Input::GetMouseButtonUp(sf::Mouse::Button b) const
{
}

void Input::ClearAll()
{
}

sf::Vector2i Input::GetMousePosition() const { return _mousePosition; }

sf::Vector2i Input::GetMouseDelta() const { return _mouseDelta; }

float Input::GetWheelDelta() const { return _wheelDelta; }

const std::u32string& Input::GetTextEntered() const { return _textEntered; }
