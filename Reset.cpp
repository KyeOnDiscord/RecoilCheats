#include "pch.h"
#include "Reset.h"

extern Cheat* cheat;

HRESULT __stdcall hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) noexcept(true)
{
	auto oReset = cheat->dx9.oReset;

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto hr = oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}