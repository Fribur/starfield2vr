#include "framework.h"
#include <hooks/CreationEngineHook.h>

std::filesystem::path dump_path;
bool minhook_inited = false;

void VRFramework::setup(HINSTANCE hModule)
{
    g_d3d12_callback_manager = new D3D12CallBackManager(hModule);
    auto logger = spdlog::basic_logger_mt("VRFramework", "vframework.log");
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info);
    init_minhook();
    CreationEngineHook::Get();

    WCHAR file_prefix[MAX_PATH] = L"";
    GetModuleFileNameW(nullptr, file_prefix, ARRAYSIZE(file_prefix));
    std::filesystem::path game_file_path = file_prefix;
    dump_path /= L"ShaderDump";
}

//void VRFramework::failed(MH_STATUS status)
//{
//        std::string response = MH_StatusToString(status);
//        auto s = response.c_str();
//        std::wstring ws(&s[0], &s[strlen(s)]);
//
//        MessageBox(0, response.c_str(), "VRFramework", 0);
//        ExitProcess(0);
//}

void VRFramework::init_minhook()
{
    if (!minhook_inited) {
        MH_STATUS status = MH_Initialize();
        if (status != MH_OK) {
            spdlog::error("Failed to initialize MinHook: {}", MH_StatusToString(status));
        }
        spdlog::info("MinHook initialized");
        minhook_inited = true;
    }
}

void VRFramework::hook_function(void* object, int index, LPVOID detour, LPVOID original)
{
    init_minhook();
    void** vtable = *reinterpret_cast<void***>(object);
    MH_STATUS status = MH_CreateHook(vtable[index], detour, reinterpret_cast<void**>(original));
    if (status != MH_OK) {
        spdlog::error("Failed to create hook: {}", MH_StatusToString(status));
        return;
    }
    status = MH_EnableHook(vtable[index]);
    if (status != MH_OK) {
        spdlog::error("Failed to enable hook: {}", MH_StatusToString(status));
        return;
    }
}


D3D12CallBackManager* VRFramework::get_callback_manager()
{
    return g_d3d12_callback_manager;
}
