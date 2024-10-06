#define DXGI_INJECTION
#include "SFVR.hpp"
//#include "Settings.h"
//#include "dgxiProxy.h"
#include "dinput8proxy.h"
//#include "framework.h"
#include <CreationEngine/CreationEngineEntry.h>

#ifndef DXGI_INJECTION
// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type) {
    case SFSE::MessagingInterface::kPostLoad: {
    }
    case SFSE::MessagingInterface::kPostPostLoad: {
    }
    case SFSE::MessagingInterface::kPostDataLoad: {
        Settings::LoadSettings();
        //Hooks::Install();
        auto ce = CreationEngineHook();

    }
    case SFSE::MessagingInterface::kPostPostDataLoad: {
    }
    default: {
    }
    }
}

// Main SFSE plugin entry point, initialize everything here
SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
    Init(sfse);

    logger::info("{} {} is loading...", Plugin::Name, Plugin::Version.string());

    if (const auto messaging{ SFSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", Plugin::Name);

    return true;
}

#else
void InitThread(HINSTANCE hModule) {
//    Settings::LoadSettings();
//    VRFramework::setup(hModule);
//    hook_d3d12();
    g_framework = std::make_unique<SFVR>(hModule);
//    g_d3d12_callback_manager = new D3D12CallBackManager(hModule);
    CreationEngineEntry::Get();
//    VRFramework::XInputHook::Get()->hook_gaming_input();
    //Hooks::Install();
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
//        do {
//            Sleep(250);
//        } while (!IsDebuggerPresent());
//        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitThread, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif
