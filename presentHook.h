#pragma once
#include "pch.h"
HRESULT __stdcall hkPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion);