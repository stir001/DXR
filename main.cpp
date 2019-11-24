#include <Windows.h>
#include "GameApp.h"

int WINAPI WinMain(_In_ HINSTANCE inst, _In_opt_  HINSTANCE hpreInst, _In_ LPSTR lpstr, _In_ int nCmd)
{
	GameApp app;
	app.Init(inst);
	app.Run();
	app.Terminate();

	return 0;
}
