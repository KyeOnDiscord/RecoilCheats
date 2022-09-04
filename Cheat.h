#pragma once

#include "pch.h"
#include "dx9.h"
#include "hook.h"
#include "vector.h"
#include "sdk/sdk.h"

class CCSPlayer;

class Cheat
{

private:

	void InitModules();

	//DirectX
	void InitDirectX9();
	IDirect3DDevice9Vtbl* dx9Vtable;
	void InitEndSceneHook();
	void InitInterfaces();

public:

	//Initialzies the hack
	void Init();

	//Updates the hack, called multiples times per frame
	void Update();

	bool WorldToScreen(const Vec3& position, Vec2& screenPosition);

	HWND window;
	Vec2 WindowSize = Vec2();

	sdk::VMatrix* viewMatrix = nullptr;
	CCSPlayer* LocalPlayer = nullptr;

	struct Modules
	{
		HMODULE client;
		HMODULE engine;
		HMODULE inputSystem;
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
		bool ShowFPS = false;
		bool ShowPos = false;
		bool ShowVelocity = false;
		bool Watermark = true;
		bool ThirdPerson = false;
		bool Bhop = false;
		int FOV = 90;
	}settings;

	struct esdeekay
	{
		sdk::IClientEntityList* ClientEntityList;
		sdk::IVEngineClient* EngineClient;
		sdk::IBaseClientDLL* BaseClientDLL;
		sdk::IInputSystem* InputSystem;

	}interfaces;

};