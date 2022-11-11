#include "pch.h"
#include "Cheat.h"
#include "sdk/sdk.h"
#include <D3dx9tex.h>
#include <chrono>
#pragma comment(lib, "D3dx9")
#include "Entity.h"

extern Cheat* cheat;


void Cheat::Init()
{
	this->InitModules();
	this->InitInterfaces();
	this->InitOffsets();
	this->InitDirectX9();
	this->InitHooks();
}

void LeftHandKnife();
void NoRecoil();

void Cheat::Update()
{
	try
	{


		if (GetAsyncKeyState(VK_F1) & 1)
		{
			this->settings.ShowMenu = !settings.ShowMenu;


			this->interfaces.InputSystem->resetInputState();
			//Disables clicking on menu, etc when menu is shown
			this->interfaces.InputSystem->EnableInput(!this->settings.ShowMenu);

			this->interfaces.EngineClient->ClientCmd_Unrestricted(this->settings.ShowMenu ? skCrypt("showconsole") : skCrypt("hideconsole"));
		}
		uintptr_t clientState = *(uintptr_t*)((uintptr_t)this->modules.engine + this->offsets.dwClientState);
		int gameState = *(uintptr_t*)(clientState + this->offsets.dwClientState_State);


		if (gameState == 6 && cheat->LocalPlayer != nullptr && *cheat->LocalPlayer->m_iHealth() > 0)
		{
			//cheat->viewMatrix = (sdk::VMatrix*)cheat->interfaces.EngineClient->WorldToScreenMatrix();

			memcpy(&viewMatrix, (BYTE*)(uintptr_t(cheat->modules.client) + cheat->offsets.dwViewMatrix), sizeof(viewMatrix));

			if (cheat->settings.LeftHandKnife)
				LeftHandKnife();

			if (cheat->settings.NoRecoil)
				NoRecoil();


			if (cheat->settings.FOV != *cheat->LocalPlayer->m_iDefaultFOV())
				*cheat->LocalPlayer->m_iDefaultFOV() = cheat->settings.FOV;

			if (cheat->settings.Bhop)
			{
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
				uintptr_t* ForceJump = (uintptr_t*)(uintptr_t(cheat->modules.client) + cheat->offsets.dwForceJump);

				if (GetAsyncKeyState(VK_SPACE) && (*cheat->LocalPlayer->m_fFlags() & FL_ONGROUND))
				{
					*ForceJump = 6;
				}
				else
				{
					*ForceJump = 4;
				}
			}
		}


		this->dx9.UpdateOverlayPosition();
	}
	catch (...)
	{

	}
}

void Cheat::InitModules()
{
	this->modules.client = GetModuleHandle(skCrypt(L"client"));
	this->modules.engine = GetModuleHandle(skCrypt(L"engine"));
	this->modules.inputSystem = GetModuleHandle(skCrypt(L"inputsystem"));
	this->modules.gameoverlayrenderer = GetModuleHandle(skCrypt(L"gameoverlayrenderer"));
}

void Cheat::InitDirectX9()
{
	void* d3d9Device[119];
	this->dx9.GetD3D9Device(d3d9Device, sizeof(d3d9Device));

	this->dx9Vtable = (IDirect3DDevice9Vtbl*)d3d9Device;
}

void Cheat::InitHooks()
{
	this->hooks.endscene = Hook::Hook((PBYTE)this->dx9Vtable->EndScene, (PBYTE)hkEndScene, (PBYTE)&this->dx9.oEndScene, 7);
	this->hooks.reset = Hook::Hook((PBYTE)this->dx9Vtable->Reset, (PBYTE)hkReset, (PBYTE)&this->dx9.oReset, 7);

	//static PBYTE presentAddress = this->GetSignature(this->modules.gameoverlayrenderer, "\xC7\x45?????\xFF\x15????\x8B\xD8", false,{}, 2);
	//uintptr_t* presentAddress = (uintptr_t*)this->GetSignature(this->modules.gameoverlayrenderer, "C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B D8", false,{}, 2);

	//cheat->dx9.oPresent = (tPresent)(presentAddress);

	//presentAddress = (uintptr_t*)(&hkPresent);


	//std::cout << &hkPresent << std::endl;
}

void Cheat::InitInterfaces()
{
	//client.dll interfaces
	this->interfaces.ClientEntityList = (sdk::IClientEntityList*)sdk::GetInterface(this->modules.client, skCrypt("VClientEntityList003"));
	this->interfaces.BaseClientDLL = (sdk::IBaseClientDLL*)sdk::GetInterface(this->modules.client, skCrypt("VClient018"));
	this->interfaces.InputSystem = (sdk::IInputSystem*)sdk::GetInterface(this->modules.inputSystem, skCrypt("InputSystemVersion001"));

	//engine.dll interfaces
	this->interfaces.EngineClient = (sdk::IVEngineClient*)sdk::GetInterface(this->modules.engine, skCrypt("VEngineClient014"));


	this->interfaces.WeaponSystem = *(sdk::IWeaponSystem**)(this->PatternScan(this->modules.client, skCrypt("8B 35 ? ? ? ? FF 10 0F B7 C0")) + 2);



}


void Cheat::InitOffsets()
{
	//Engine.dll offsets
	this->offsets.dwClientState = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0"), true, { 1 });
	this->offsets.dwClientState_State = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("83 B8 ? ? ? ? ? 0F 94 C0 C3"), false, { 2 });
	this->offsets.dwClientState_ViewAngles = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("F3 0F 11 86 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 86"), false, { 4 });


	//Client.dll offsets

	this->offsets.m_bDormant = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("8A 81 ? ? ? ? C3 32 C0"), false, { 2 }, 8);
	this->offsets.dwForceJump = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02"), true, { 2 });
	this->offsets.dwViewMatrix = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"), true, { 3 }, 176);
	this->offsets.m_pStudioHdr = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("8B B6 ? ? ? ? 85 F6 74 05 83 3E 00 75 02 33 F6 F3 0F 10 44 24"), false, { 2 });
}


BOOL CALLBACK enumWind(HWND handle, LPARAM lp)
{
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);

	if (GetCurrentProcessId() != procId)
		return TRUE;

	cheat->window = handle;
	return FALSE;
}



void Cheat::directx9::UpdateOverlayPosition()
{
	int width, height;
	cheat->interfaces.EngineClient->GetScreenSize(width, height);
	cheat->WindowSize = Vec2((float)width, (float)height);
}


HWND GetProcessWindow()
{
	cheat->window = NULL;
	EnumWindows(enumWind, NULL);
	//UpdateOverlayPosition();

	//WindowHeight -= 29;
	//WindowWidth -= 5;
	return cheat->window;
}


bool Cheat::directx9::GetD3D9Device(void** pTable, size_t size)
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	IDirect3DDevice9* pDummyDevice = nullptr;
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}


	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}




// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromFile(LPDIRECT3DDEVICE9 pDevice, const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(pDevice, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

bool LoadTextureFromMemory(IDirect3DDevice9* pDevice, LPVOID pSrcData, IDirect3DTexture9* out_texture, int* out_width, int* out_height)
{
	// Load texture from memory

	HRESULT hr = D3DXCreateTextureFromFileInMemory(pDevice, pSrcData, sizeof(pSrcData), &out_texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	out_texture->GetLevelDesc(0, &my_image_desc);
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

//https://www.unknowncheats.me/forum/counterstrike-global-offensive/291757-fps-indicator.html
int FrameRate()
{

	static int fps = 0;
	static int count = 0;
	using namespace std::chrono;
	auto now = high_resolution_clock::now();
	static auto last = high_resolution_clock::now();
	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}


bool WorldToScreenCalculation(Vec3 pos, Vec2& screen, int windowWidth, int windowHeight)
{
	Vec4 clipCoords;
	clipCoords.x = pos.x * cheat->viewMatrix[0] + pos.y * cheat->viewMatrix[1] + pos.z * cheat->viewMatrix[2] + cheat->viewMatrix[3];
	clipCoords.y = pos.x * cheat->viewMatrix[4] + pos.y * cheat->viewMatrix[5] + pos.z * cheat->viewMatrix[6] + cheat->viewMatrix[7];
	clipCoords.z = pos.x * cheat->viewMatrix[8] + pos.y * cheat->viewMatrix[9] + pos.z * cheat->viewMatrix[10] + cheat->viewMatrix[11];
	clipCoords.w = pos.x * cheat->viewMatrix[12] + pos.y * cheat->viewMatrix[13] + pos.z * cheat->viewMatrix[14] + cheat->viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;


	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}


bool Cheat::WorldToScreen(const Vec3& in, Vec2& out)
{
	if (cheat->viewMatrix != nullptr)
	{
		return WorldToScreenCalculation(in, out, (int)cheat->WindowSize.x, (int)cheat->WindowSize.y);
	}
	else
	{
		return false;
	}

}




Vec3* Cheat::GetViewAngles()
{
	uintptr_t dwClientState = *(uintptr_t*)((uintptr_t)this->modules.engine + this->offsets.dwClientState);

	return (Vec3*)(dwClientState + this->offsets.dwClientState_ViewAngles);
}