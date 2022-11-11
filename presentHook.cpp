#include "presentHook.h"
#include "pch.h"
extern Cheat* cheat;

HRESULT __stdcall hkPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion)
{
	std::cout << "Present hook" << std::endl;

	return cheat->dx9.oPresent(device, src, dest, windowOverride, dirtyRegion);
}