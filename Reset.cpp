#include "pch.h"
#include "Reset.h"

extern Cheat* cheat;

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return cheat->dx9.oReset(pDevice, pPresentationParameters);
}