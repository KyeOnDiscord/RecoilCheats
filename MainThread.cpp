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

	cheat->hooks.endscene.EnableHook();

	auto testString = skCrypt(L"TestString");	// encrypted at compile-time
	// or skCrypt_key to set the keys manually

	wprintf(testString);                            // automatic decryption on usage (alternatively .decrypt())

	while (!(GetKeyState(VK_END) & 0x8000))
	{
		cheat->Update();
	}

	
	cheat->hooks.endscene.DisableHook();
	Sleep(100);
	SetWindowLongPtr(cheat->window, GWLP_WNDPROC, (LONG_PTR)cheat->dx9.oriWndProc);
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	//wait a bit
	Sleep(300);
#ifdef UseConsole
	fclose(f);
	FreeConsole();
#endif // UseConsole


	FreeLibraryAndExitThread(hModule, 0);
}