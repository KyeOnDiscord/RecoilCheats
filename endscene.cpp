#include "pch.h"
#include "endscene.h"

extern Cheat* cheat;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (cheat->settings.ShowMenu)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}
	}
	return CallWindowProc(cheat->dx9.oriWndProc, hWnd, uMsg, wParam, lParam);
}



void InitImGui(IDirect3DDevice9* pDevice) {
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	cheat->window = CP.hFocusWindow;

	// IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.Fonts->AddFontDefault();


	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle* style = &ImGui::GetStyle();
	colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.6f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
	colors[ImGuiCol_Separator] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab] = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
	colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	style->FramePadding = ImVec2(4, 2);
	style->ItemSpacing = ImVec2(10, 2);
	style->IndentSpacing = 12;
	style->ScrollbarSize = 10;

	//style->WindowRounding = 4;
	style->FrameRounding = 4;
	style->PopupRounding = 4;
	style->ScrollbarRounding = 6;
	style->GrabRounding = 4;
	style->TabRounding = 4;

	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	//style->WindowMenuButtonPosition = ImGuiDir_None;

	style->DisplaySafeAreaPadding = ImVec2(4, 4);


	ImGui_ImplWin32_Init(cheat->window);
	ImGui_ImplDX9_Init(pDevice);
	cheat->dx9.oriWndProc = (WNDPROC)SetWindowLongPtr(cheat->window, GWLP_WNDPROC, (LONG_PTR)hWndProc);
	cheat->dx9.UpdateOverlayPosition();
}


int FrameRate();
HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr)
		return cheat->dx9.oEndScene(pDevice);

	static bool Initialized = false;
	int ChisatoIMGWidth = 666;
	int ChisatoIMGHeight = 375;
	static IDirect3DTexture9* texture = nullptr;
	if (!Initialized)
	{
		InitImGui(pDevice);
		
		
		if (texture == nullptr)
		{
			HRESULT result = D3DXCreateTextureFromFile(pDevice, skCrypt(L"chisatoEDcrop.png"), &texture);
		}
		
		
		Initialized = true;
	}
	

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	cheat->dx9.drawlist = ImGui::GetBackgroundDrawList();
	static ImVec2 windowSize = ImVec2(600, 300);
	static ImVec2 windowPos = ImVec2(60, 60);
	ImGuiIO io = ImGui::GetIO();
	
	static float bgOpacity = 0.1;
	ImGui::SetNextWindowBgAlpha(bgOpacity);
	
	if (cheat->settings.ShowMenu && ImGui::Begin(skCrypt("Recoil Cheats | By Kye#5000"), (bool*)false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
		if (bgOpacity <= 0.5f)
			bgOpacity += 0.01f;

		
		ImGui::SetWindowSize(windowSize, ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos(windowPos, ImGuiCond_FirstUseEver);

		ImGui::Text(skCrypt("Toggle Menu with F1"));
		ImGui::Text(skCrypt("Eject hack with END"));
		
		#define Column1Size 150
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, Column1Size);
		static ImVec4 active = ImVec4((float)50 / 255, (float)49 / 255, (float)50 / 255, 1.f);
		static ImVec4 inactive = ImVec4((float)31 / 255, (float)30 / 255, (float)31 / 255, 1.f);
		static int TabButton = 0;
		{
			//Left Column
			const char* buttons[] = { skCrypt("Visuals"), skCrypt("Aimbot"), skCrypt("Triggerbot"), skCrypt("Misc"), skCrypt("Skin Changer")};

			for (size_t i = 0; i < IM_ARRAYSIZE(buttons); i++)
			{
				ImGui::Spacing();
				ImGui::PushStyleColor(ImGuiCol_Button, TabButton == i ? active : inactive);
				if (ImGui::Button(buttons[i], ImVec2(Column1Size - 15, 39)))
					TabButton = i;

			}



			ImGui::PopStyleColor(IM_ARRAYSIZE(buttons));
		}

		ImGui::SetCursorPosX(Column1Size / 2 - 30);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
		ImGui::Text(skCrypt("Kye#5000"));

		ImGui::NextColumn();


		{
			//Right Column

			switch (TabButton)
			{
			case 3:
				ImGui::Checkbox(skCrypt("Show FPS"), &cheat->settings.ShowFPS);

				break;
			}
		}
		
		

		ImGui::End();

		ImGui::ShowDemoWindow();
	}
	else
	{
		if (bgOpacity >= 0.0f)
			bgOpacity -= 0.01f;
	}
	
	ImU32 greyBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.1f, 0.1f, bgOpacity));
	cheat->dx9.drawlist->AddRectFilled(ImVec2(0, 0), ImVec2(cheat->WindowSize.x, cheat->WindowSize.y), greyBg);
	cheat->dx9.drawlist->AddImage((void*)texture, ImVec2((cheat->WindowSize.x - ChisatoIMGWidth / 1.1) + 100, cheat->WindowSize.y - ChisatoIMGHeight / 1.1), ImVec2(cheat->WindowSize.x + 100, cheat->WindowSize.y), ImVec2(0,0),ImVec2(1,1), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, bgOpacity)));
	
	ImGui::SetNextWindowBgAlpha(0.1f);
	if (cheat->settings.ShowFPS && ImGui::Begin(skCrypt("FPS Counter"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav))
	{
		
		ImGui::SetWindowSize(ImVec2(100,10), ImGuiCond_Always);
		ImGui::SetWindowPos(ImVec2(25,25), ImGuiCond_Always);

		
		ImGui::Text(skCrypt("FPS: "));
		ImGui::SameLine();
		ImGui::Text(std::to_string(io.Framerate).c_str());
		//Endscene is called twice every frame so half it to get accurate fps

		ImGui::End();
	}
	
	
	
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return cheat->dx9.oEndScene(pDevice);
}