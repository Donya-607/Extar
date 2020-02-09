#pragma once

#include "DxLib.h"


bool    ImGui_ImplDxLib_Init();
void    ImGui_ImplDxLib_Shutdown();
void    ImGui_ImplDxLib_NewFrame();

bool    ImGui_ImplDxLib_CreateDeviceObjects();
void    ImGui_ImplDxLib_InvalidateDeviceObjects();

LRESULT ImGui_ImplDxLib_WndProcHandler(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);


