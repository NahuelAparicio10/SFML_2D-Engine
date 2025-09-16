#pragma once
#include <bitset>
#include "SFML/Window/Event.hpp"
#include <array>

class GamepadState
{
public:
    static constexpr unsigned int MaxJoysticks = 8;
    static constexpr unsigned int MaxButtons = 32;
    static constexpr std::size_t AxisCount = 8; // X,Y,Z,R,U,V,POV X,POV Y


    void BeginFrame();
    void Consume(const sf::Event& ev);
    void EndFrame();

    bool IsConnected(unsigned int id) const;

    // Buttons
    bool GetButton(unsigned int id, unsigned int button) const;
    bool GetButtonDown(unsigned int id, unsigned int button) const;
    bool GetButtonUp(unsigned int id, unsigned int button) const;

    // Normalized axis [-1, 1] with deadzone applied
    float GetAxis(unsigned int id, sf::Joystick::Axis axis) const;
    void  SetDeadzone(float dz);
    float GetDeadzone() const;

private:
    struct Pad
    {
        std::bitset<MaxButtons> now{}, prev{};
        std::array<float, AxisCount> axes{}; // normalized [-1,1]
        bool connected = false;
    };

    std::array<Pad, MaxJoysticks> _pads{};
    float _deadzone = 0.15f;

    static std::size_t AxisIndex(sf::Joystick::Axis ax);
    static float Normalize100(float v);
    float ApplyDeadZone(float v) const;
    void ResetPad(unsigned int id);
};


