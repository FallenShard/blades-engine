#include "Core/Application.h"

#if defined(_DEBUG)
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
#endif
    fsi::Application application;
    application.run();
    return 0;
}