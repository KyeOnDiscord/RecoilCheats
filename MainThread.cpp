#include "pch.h"
#include "MainThread.h"

#if _DEBUG
//#define UseConsole true
#endif

//Everything will be inside the cheat class pointer so we only have 1 instance of it
Cheat* cheat;



DWORD WINAPI MainThread(HMODULE hModule)
{
#ifdef UseConsole
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif // UseConsole
	
	cheat = new Cheat();
	cheat->Init();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	cheat->hooks.endscene.EnableHook();

	//Disable input on first launch because the menu is open
	cheat->interfaces.InputSystem->EnableInput(!cheat->settings.ShowMenu);
	cheat->interfaces.EngineClient->ClientCmd_Unrestricted("showconsole");
	
	while (!(GetKeyState(VK_END) & 0x8000))
	{
		cheat->Update();

		Sleep(1000 / 120);
	}
	cheat->settings.ShowMenu = false;
	//Ejecting the cheat
	cheat->hooks.endscene.DisableHook(); //Disable the end scene hook
	cheat->interfaces.InputSystem->EnableInput(true); //Enable the input again so the user can click on buttons
	cheat->interfaces.InputSystem->resetInputState();
	SetWindowLongPtr(cheat->window, GWLP_WNDPROC, (LONG_PTR)cheat->dx9.oriWndProc);
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	//wait a bit
	Sleep(500);
#ifdef UseConsole
	fclose(f);
	FreeConsole();
#endif // UseConsole


	FreeLibraryAndExitThread(hModule, 0);
}