
#include <Windows.h>
#include "MainWindow.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	MainWindow mainWindow(hInst, hPrevInst, args, ncmdshow);
	mainWindow.Run();

	return 0;
}

