#include "Core/Application.h"


#if defined(_DEBUG)
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
    Application application;
    application.run();
    return 0;
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    Application application;
    application.run();

    return 0;
}

#endif