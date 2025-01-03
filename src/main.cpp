#ifdef _WIN32
#include <windows.h>
#endif

#include "Application.h"

#if defined(_WIN32) && defined(RELEASE_BUILD)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    gl_cv_app::Application app;
    app.run();
}