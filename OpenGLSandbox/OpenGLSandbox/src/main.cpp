#include "Core/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    Application application;
    application.run();

    return 0;
}

#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
    Application application;
    application.run();
    return 0;
}