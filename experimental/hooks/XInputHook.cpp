#pragma once
#include "XInputHook.h"
#include <Xinput.h>
#include <framework.h>
#include <hstring.h>
#include <winrt/Windows.Gaming.Input.h>

typedef DWORD(WINAPI* XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
XInputGetState_t originalXInputGetState = nullptr;

DWORD WINAPI detourXInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
    return VRFramework::XInputHook::Get()->XInputGetState(dwUserIndex, pState);
}

typedef WINUSERAPI UINT(WINAPI* GetRawInputData_t)(_In_ HRAWINPUT hRawInput, _In_ UINT uiCommand, _Out_writes_bytes_to_opt_(*pcbSize, return) LPVOID pData, _Inout_ PUINT pcbSize,
                                                   _In_ UINT cbSizeHeader);
GetRawInputData_t  originalGetRawInputData = nullptr;

UINT WINAPI detourGetRawInputData(_In_ HRAWINPUT hRawInput, _In_ UINT uiCommand, _Out_writes_bytes_to_opt_(*pcbSize, return) LPVOID pData, _Inout_ PUINT pcbSize,
                                  _In_ UINT cbSizeHeader)
{
    return originalGetRawInputData(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);
}

using RoGetActivationFactory_pfn                          = HRESULT(WINAPI*)(HSTRING, REFIID, void**);
RoGetActivationFactory_pfn originalRoGetActivationFactory = nullptr;

HRESULT

WINAPI
RoGetActivationFactoryDetour(_In_ HSTRING activatableClassId, _In_ REFIID iid, _COM_Outptr_ void** factory)
{
    spdlog::info("RoGetActivationFactory called from proxy");
    return originalRoGetActivationFactory(activatableClassId, iid, factory);
}

HMODULE g_xinupu1_4 = nullptr;
HMODULE g_user32    = nullptr;
HMODULE g_combase   = nullptr;

void VRFramework::XInputHook::InitHooks()
{
    init_minhook();
    spdlog::info("XInputHook::InitHooks()");
    while ((g_xinupu1_4 = GetModuleHandle("XInput9_1_0")) == NULL) {
        Sleep(50);
    }

    auto pTargetXInputGetState = GetProcAddress(g_xinupu1_4, "XInputGetState");
    if (!pTargetXInputGetState) {
        spdlog::error("Failed to get XInputGetState function");
        return;
    }

    auto status = MH_CreateHook(pTargetXInputGetState, &detourXInputGetState, reinterpret_cast<void**>(&originalXInputGetState));
    if (status != MH_OK) {
        spdlog::error("Failed to hook XInputGetState: {}", MH_StatusToString(status));
        return;
    }

    status = MH_EnableHook(pTargetXInputGetState);

    if (status != MH_OK) {
        spdlog::error("Failed to hook XInputGetState: {}", MH_StatusToString(status));
    }
    spdlog::info("XInputHook::InitHooks() done");
}

void VRFramework::XInputHook::HookGerRawInput()
{
    init_minhook();
    spdlog::info("HookGerRawInput::InitHooks()");
    while ((g_user32 = GetModuleHandle("User32")) == NULL) {
        Sleep(50);
    }
    auto pTargetGetRawInputBuffer = GetProcAddress(g_user32, "GetRawInputData");

    if (!pTargetGetRawInputBuffer) {
        spdlog::error("Failed to get XInputGetState function");
        return;
    }

    auto status = MH_CreateHook(pTargetGetRawInputBuffer, &detourGetRawInputData, reinterpret_cast<void**>(&originalGetRawInputData));
    if (status != MH_OK) {
        spdlog::error("Failed to hook XInputGetState: {}", MH_StatusToString(status));
        return;
    }

    status = MH_EnableHook(pTargetGetRawInputBuffer);

    if (status != MH_OK) {
        spdlog::error("Failed to hook GetRawInputBuffer: {}", MH_StatusToString(status));
    }
    spdlog::info("HookGerRawInput::InitHooks() done");
}

void VRFramework::XInputHook::hook_gaming_input()
{
    init_minhook();
    spdlog::info("XInputHook::hook_gaming_input()");
    while ((g_combase = GetModuleHandle("combase")) == NULL) {
        Sleep(50);
    }
    auto pTargetGetActivationFactory = GetProcAddress(g_combase, "RoGetActivationFactory");

    if (!pTargetGetActivationFactory) {
        spdlog::error("Failed to get RoGetActivationFactory function");
        return;
    }

    auto status = MH_CreateHook(pTargetGetActivationFactory, &RoGetActivationFactoryDetour, reinterpret_cast<void**>(&originalRoGetActivationFactory));
    if (status != MH_OK) {
        spdlog::error("Failed to hook RoGetActivationFactory: {}", MH_StatusToString(status));
        return;
    }

    status = MH_EnableHook(pTargetGetActivationFactory);

    if (status != MH_OK) {
        spdlog::error("Failed to hook RoGetActivationFactory: {}", MH_StatusToString(status));
    }
}

DWORD VRFramework::XInputHook::XInputGetState(DWORD i, _XINPUT_STATE* pState)
{
    return originalXInputGetState(i, pState);
}

HRESULT VRFramework::XInputHook::RoGetActivationFactory(HSTRING hstring, const IID& guid, void** pVoid)
{
    spdlog::info("RoGetActivationFactory called from proxy");
    return originalRoGetActivationFactory(hstring, guid, pVoid);
}
