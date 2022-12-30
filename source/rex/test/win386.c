#include <windows.h>

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Hello world", "Open Watcom C/C++ for Windows", MB_OK | MB_TASKMODAL);
	return(0);
}
