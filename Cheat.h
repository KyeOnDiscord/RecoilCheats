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
	void InitHooks();
	void InitInterfaces();
	void InitOffsets();
	std::uint8_t* PatternScan(void* module, const char* signature);
public:

	//Initialzies the hack
	void Init();

	//Updates the hack, called multiples times per frame
	void Update();
	
	std::uint8_t* GetSignature(HMODULE module, const char* signature, bool relative, std::vector<int> offsets, int extra);
	bool WorldToScreen(const Vec3& position, Vec2& screenPosition);
	Vec3* GetViewAngles();

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
		Hook::Hook reset;
	} hooks;

	struct directx9
	{
	public:
		bool GetD3D9Device(void** pTable, size_t size);
		void UpdateOverlayPosition();

		tEndScene oEndScene = nullptr;
		tReset oReset = nullptr;

		WNDPROC oriWndProc = NULL;
		ImDrawList* drawlist;
	} dx9;

	struct Settings
	{
		bool ShowMenu = true;
		bool ShowFPS = false;
		bool ShowPos = false;
		bool ShowVelocity = false;
		bool ShowViewAngles = false;
		bool Watermark = true;
		bool ThirdPerson = false;
		bool LeftHandKnife = false;
		bool Bhop = false;
		bool NoRecoil = false;
		int Snaplines = 0;
		int FOV = 90;
	}settings;

	struct esdeekay
	{
		sdk::IClientEntityList* ClientEntityList;
		sdk::IVEngineClient* EngineClient;
		sdk::IBaseClientDLL* BaseClientDLL;
		sdk::IInputSystem* InputSystem;
		sdk::IWeaponSystem* WeaponSystem;

	}interfaces;


	struct offsets
	{
		uintptr_t dwClientState = 0;
		uintptr_t dwClientState_State = 0;
		uintptr_t dwClientState_ViewAngles = 0;
		uintptr_t m_bDormant = 0;
	}offsets;
};