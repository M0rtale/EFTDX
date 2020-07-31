#pragma once

#include <map>
#include <algorithm>
#include <functional>
#include <xmmintrin.h>
#include <emmintrin.h>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"
#include "crappy.h"


Vector3 WorldToScreen(Vector3 origin);
void updateCamera(ULONG64 camera);
std::string VariableText(const char* format, ...);
Vector3 get_bone_pos(uint64_t);
//extern ImFont* m_pTabs;
extern ImFont* m_pDefault;
extern ImFont* m_pChinese;
extern float _Test_Type_Float;
extern int s_width;
extern int s_height;
extern D3DXMATRIX views;