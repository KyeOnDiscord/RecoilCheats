#include "pch.h"
#include "Cheat.h"
#include "sdk/sdk.h"
extern Cheat* cheat;


void Cheat::Init()
{
	this->InitModules();
	this->InitInterfaces();
	this->InitDirectX9();
	this->InitEndSceneHook();
}


void Cheat::Update()
{
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		this->settings.ShowMenu = !settings.ShowMenu;

		
		cheat->interfaces.InputSystem->resetInputState();
		//Disables clicking on menu, etc when menu is shown
		cheat->interfaces.InputSystem->EnableInput(!cheat->settings.ShowMenu);
		
		cheat->interfaces.EngineClient->ClientCmd_Unrestricted(cheat->settings.ShowMenu ? "showconsole" : "hideconsole");
	}

	cheat->dx9.UpdateOverlayPosition();
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

void Cheat::InitEndSceneHook()
{
	this->hooks.endscene = Hook::Hook((PBYTE)this->dx9Vtable->EndScene, (PBYTE)hkEndScene, (PBYTE)&cheat->dx9.oEndScene, 7);
}

void Cheat::InitInterfaces()
{
	//client.dll interfaces
	this->interfaces.ClientEntityList = (sdk::IClientEntityList*)sdk::GetInterface(cheat->modules.client, skCrypt("VClientEntityList003"));
	this->interfaces.BaseClientDLL = (sdk::IBaseClientDLL*)sdk::GetInterface(cheat->modules.client, skCrypt("VClient018"));
	this->interfaces.InputSystem = (sdk::IInputSystem*)sdk::GetInterface(cheat->modules.inputSystem, skCrypt("InputSystemVersion001"));
	
	//engine.dll interfaces
	this->interfaces.EngineClient = (sdk::IVEngineClient*)sdk::GetInterface(cheat->modules.engine, skCrypt("VEngineClient014"));
	
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
	static int width, height;
	cheat->interfaces.EngineClient->GetScreenSize(width, height);	
	cheat->WindowSize = Vec2((float)width,(float)height);
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


#include <D3dx9tex.h>
#include <chrono>
#pragma comment(lib, "D3dx9")

// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromFile(IDirect3DDevice9* pDevice,const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
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
bool FrustomTransform(const sdk::VMatrix& worldToSurface, const Vec3& point, Vec2& screen) {
	
	screen.x = worldToSurface.m[0][0] * point.x + worldToSurface.m[0][1] * point.y + worldToSurface.m[0][2] * point.z + worldToSurface.m[0][3];
	screen.y = worldToSurface.m[1][0] * point.x + worldToSurface.m[1][1] * point.y + worldToSurface.m[1][2] * point.z + worldToSurface.m[1][3];
	auto w = worldToSurface.m[3][0] * point.x + worldToSurface.m[3][1] * point.y + worldToSurface.m[3][2] * point.z + worldToSurface.m[3][3];

	bool facing{};
	if (w < 0.001f)
	{
		facing = false;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		facing = true;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return facing;
}


bool Cheat::WorldToScreen(const Vec3& position, Vec2& screenPosition) 
{

	sdk::VMatrix worldToScreenMatrix = (sdk::VMatrix)this->interfaces.EngineClient->WorldToScreenMatrix();

	auto facing{ FrustomTransform(worldToScreenMatrix, position, screenPosition) };

	int screenWidth = this->WindowSize.x;
	int screenHeight = this->WindowSize.y;
	screenPosition.x = 0.5f * (1.0f + screenPosition.x) * screenWidth;
	screenPosition.y = 0.5f * (1.0f - screenPosition.y) * screenHeight;

	auto visible{ (screenPosition.x >= 0 && screenPosition.x <= screenWidth) &&
		screenPosition.y >= 0 && screenPosition.y <= screenHeight };
	if (!facing || !visible)
	{
		screenPosition.x = -640;
		screenPosition.y = -640;
		return false;
	}

	return true;
}