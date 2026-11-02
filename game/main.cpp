#include <windows.h>
#include "WinBMApp.h"

using namespace Sexy;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WinBMApp anApp;
    anApp.Init();
    anApp.Start();
    anApp.Shutdown();
    return 0;
}