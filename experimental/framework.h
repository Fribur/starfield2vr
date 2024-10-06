#pragma once
#pragma warning(disable : 4996)
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
//#include <spdlog/fmt/fmt.h>
//#include <spdlog/fmt/bundled/format.h>
#include <windows.h>

//#include "mods/D3D12CallBackManager.h"
#include <MinHook.h>
#include <d3d12.h>
#include <filesystem>
#include <mods/D3D12CallBackManager.h>

extern HMODULE g_d3d12;
extern HMODULE g_xinput;
extern std::filesystem::path dump_path;

class D3D12CallBackManager;

namespace VRFramework
{
    void setup(HINSTANCE hModule);
    void init_minhook();
    //    void failed(MH_STATUS status);
    void hook_function(void* object, int index, LPVOID detour, LPVOID original);
    D3D12CallBackManager* get_callback_manager();

}
