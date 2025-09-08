#include "Time.h"
#include <algorithm>

void Time::SetFixedStep(double seconds)
{
    _fixedStep = (seconds > 0.0) ? seconds : (1.0 / 60.0);
}

void Time::SetTimeScale(float scale)
{
    _timeScale = (scale < 0.f) ? 0.f : scale;
}

void Time::Reset()
{
    _accumulator = 0.0;
    _frameDt = 0.f;
    _unscaledFrameDt = 0.f;
    _timeScale = 1.f;
    _alpha = 0.f;
    _total = 0.0;
}

void Time::Tick(float unscaledFrameDt)
{
    // Clampa dt grande (alt-tab, ventana arrastrada...)
    _unscaledFrameDt = std::clamp(unscaledFrameDt, 0.f, 0.25f);

    _frameDt = _unscaledFrameDt * _timeScale;

    // Cap del acumulador para evitar spiral of death (máx 1s)
    _accumulator = std::min(_accumulator + static_cast<double>(_frameDt), 1.0);
    _total += _frameDt;

    _alpha = static_cast<float>(_accumulator / _fixedStep);
    if (_alpha > 1.f) _alpha = 1.f;
}

bool Time::StepFixed()
{
    if (_accumulator >= _fixedStep)
    {
        _accumulator -= _fixedStep;
        return true;
    }
    return false;
}
