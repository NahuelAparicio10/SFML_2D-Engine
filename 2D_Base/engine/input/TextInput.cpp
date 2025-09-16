#include "TextInput.h"

void TextInput::BeginFrame()
{
    _buffer.clear();
}

void TextInput::Consume(const sf::Event& ev)
{
    if (const auto textEntered = ev.getIf<sf::Event::TextEntered>())
    {
        if (textEntered->unicode >= 32)
        {
            _buffer.push_back(static_cast<char32_t>(textEntered->unicode));
        }
    }
}

void TextInput::EndFrame() {}

const std::u32string& TextInput::Buffer() const { return _buffer; }
