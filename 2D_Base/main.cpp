#include "engine/app/Application.h"

int main()
{
    auto* app = new Application(AppConfig());

    app->Run();

    return 0;
}
