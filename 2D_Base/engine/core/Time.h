#pragma once

class Time
{
public:
    // Setters
    void SetFixedStep(double seconds);
    void SetTimeScale(float scale);
    void Reset();

    // Advance one frame (unscaled dt from OS clock)
    void Tick(float unscaledFrameDt);

    // Return true while there are pending fixed steps
    bool StepFixed();

    // Getters
    double GetFixedStep() const { return _fixedStep; }
    double GetAccumulator() const { return _accumulator; }
    float  GetFrameDt() const { return _frameDt; }
    float  GetUnscaledFrameDt() const { return _unscaledFrameDt; }
    float  GetTimeScale() const { return _timeScale; }
    float  GetAlpha() const { return _alpha; }
    double GetTotal() const { return _total; }

private:
    double _fixedStep = 1.0 / 60.0;
    double _accumulator = 0.f;
    float _frameDt = 0.f;
    float  _unscaledFrameDt = 0.f;  // real clock dt
    float  _timeScale = 1.f;        // 0 = pause, 1 = normal, more = full speed
    float  _alpha = 0.f;            // accumulator_ / fixedStep_
    double _total = 0.0;            // total simulated time (scaled)
};


