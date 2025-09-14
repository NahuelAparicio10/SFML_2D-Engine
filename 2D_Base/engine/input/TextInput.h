#pragma once
#include "SFML/Window/Event.hpp"

class TextInput
{
public:
    void BeginFrame();
    void Consume(const sf::Event& ev);
    void EndFrame();

    const std::u32string& Buffer() const;
private:
    std::u32string _buffer;
};


