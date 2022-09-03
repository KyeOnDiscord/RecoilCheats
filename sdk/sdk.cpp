#include "..\pch.h"
#include "sdk.h"


namespace sdk
{
	void* GetInterface(HMODULE hModule, const char* interfacename)
	{
		tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(hModule, "CreateInterface");

		int returnCode = 0;
		void* interface_ = CreateInterface(interfacename, &returnCode);

		return interface_;
	}
};