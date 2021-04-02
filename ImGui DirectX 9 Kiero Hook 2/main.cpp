#include "includes.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TlHelp32.h>

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false;

bool show_main = true;

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

bool bhop = false;
bool radar = false;

struct gameOffsets
{
	DWORD lPlayer = 0xD8C2CC;
	DWORD fJump = 0x524DEBC;
	DWORD flags = 0x104;
	DWORD velocity = 0x114;
	DWORD entityList = 0x4DA3F5C;
	DWORD isSpotted = 0x93D;
}offsets;

struct values
{
	DWORD localPlayer;
	DWORD gameModule;
	BYTE flag;
}val;

struct vector
{
	float x, y, z;
};

bool isPlayerMoving()
{
	vector playerVel = *(vector*)(val.localPlayer + offsets.velocity);
	int vel = (int)(playerVel.x + playerVel.y + playerVel.z);
	if (vel != 0)
		return true;
	else
		return false;
}


long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}
	if (GetAsyncKeyState(VK_TAB) & 1) {
		show_main = !show_main;
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_main)
	{
		ImGui::Begin("sexocheat");
		ImGui::Checkbox("BHOP", &bhop);
		ImGui::Checkbox("RADAR", &radar);
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

DWORD WINAPI BhopThread(LPVOID lp)
{
		val.gameModule = (DWORD)GetModuleHandle("client.dll");
		val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

		if (val.localPlayer == NULL)
			while (val.localPlayer == NULL)
				val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

		while (true)
		{
			if (bhop)
			{
				val.flag = *(BYTE*)(val.localPlayer + offsets.flags);

				if (isPlayerMoving())
					if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
						*(DWORD*)(val.gameModule + offsets.fJump) = 6;
			}
		}
}

DWORD WINAPI RadarThread(LPVOID lp)
{
	val.gameModule = (DWORD)GetModuleHandle("client.dll");

		while (true)
			if (radar)
				for (short int i = 0; i < 64; i++) 
				{
					val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer + i * 0x10);
					if (val.localPlayer != NULL)
						*(BOOLEAN*)(val.localPlayer + offsets.isSpotted) = true;
				}

}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, BhopThread, hMod, 0, nullptr);
		// CreateThread(nullptr, 0, RadarThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
