#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include "../../game/input/InputEnums.h"
#include "SFML/Window/Mouse.hpp"


struct ActionBindings
{
    std::unordered_map<Actions, sf::Keyboard::Scancode> keys;
    std::unordered_map<Actions, sf::Mouse::Button> buttons;
};

struct AxisBindings
{
    std::unordered_map<Axis, sf::Keyboard::Scancode> positive, negative;
};