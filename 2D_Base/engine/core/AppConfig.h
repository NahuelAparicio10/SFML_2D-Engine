#include <string>

struct AppConfig
{
    std::string appName = "SFML 2D Base Engine";
    unsigned int width = 1920;
    unsigned int height = 1080;
    bool vsync = true;
    unsigned int fpsCap = 60;
    float fixedStep = 1.f / 60.f;
};