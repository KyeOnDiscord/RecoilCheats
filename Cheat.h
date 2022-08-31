#pragma once

#include "pch.h"
#include "dx9.h"
#include "hook.h"
#include "vector.h"

class Cheat
{

private:

	void InitModules();

	//DirectX
	void InitDirectX9();
	IDirect3DDevice9Vtbl* dx9Vtable;
	void InitEndSceneHook();

	
public:

	//Initialzies the hack
	void Init();

	//Updates the hack, called multiples times per frame
	void Update();

	
	HWND window;
	Vec2 WindowSize = Vec2();
	struct Modules
	{
		HMODULE client;
		HMODULE engine;
	} modules;

	struct hooooks
	{
		Hook::Hook endscene;
	} hooks;

	struct directx9
	{		
	public:
		bool GetD3D9Device(void** pTable, size_t size);
		void UpdateOverlayPosition();

		tEndScene oEndScene = nullptr;

		WNDPROC oriWndProc = NULL;
		ImDrawList* drawlist;
	} dx9;

	struct Settings
	{
		bool ShowMenu = true;
	}settings;

	
};