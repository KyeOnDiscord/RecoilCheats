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
		cheat->viewMatrix = (sdk::VMatrix*)cheat->interfaces.EngineClient->WorldToScreenMatrix();
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

void Cheat::InitModules()
{
	this->modules.client = GetModuleHandle(skCrypt(L"client"));
	this->modules.engine = GetModuleHandle(skCrypt(L"engine"));
	this->modules.inputSystem = GetModuleHandle(skCrypt(L"inputsystem"));
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
	this->offsets.dwClientState = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0"), true, { 1 }, 0);
	this->offsets.dwClientState_State = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("83 B8 ? ? ? ? ? 0F 94 C0 C3"), false, { 2 }, 0);
	this->offsets.dwClientState_ViewAngles = (uintptr_t)this->GetSignature(this->modules.engine, skCrypt("F3 0F 11 86 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 86"), false, { 4 }, 0);

	//Client.dll offsets

	this->offsets.m_bDormant = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("8A 81 ? ? ? ? C3 32 C0"), false, { 2 }, 8);
	this->offsets.dwForceJump = (uintptr_t)this->GetSignature(this->modules.client, skCrypt("8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02"), true, { 2 }, 0);
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


bool WorldToScreenCalculation(Vec3 in, Vec2& screen, sdk::VMatrix matrix, int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = matrix.m[0][0] * in.x + matrix.m[0][1] * in.y + matrix.m[0][2] * in.z + matrix.m[0][3];
	clipCoords.y = matrix.m[1][0] * in.x + matrix.m[1][1] * in.y + matrix.m[1][2] * in.z + matrix.m[1][3];
	clipCoords.z = 0.0f;
	clipCoords.w = matrix.m[3][0] * in.x + matrix.m[3][1] * in.y + matrix.m[3][2] * in.z + matrix.m[3][3];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
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
		return WorldToScreenCalculation(in, out, *cheat->viewMatrix, (int)cheat->WindowSize.x, (int)cheat->WindowSize.y);
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