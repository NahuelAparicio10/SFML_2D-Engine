#pragma once
#include "Component.h"
#include "SFML/System/Vector2.hpp"

class Transform : public Component
{
    public:
    std::type_index GetType() const override;

    sf::Vector2f position {0.f, 0.f};
    sf::Angle rotation {};
    sf::Vector2f scale {1.f, 1.f};
};


