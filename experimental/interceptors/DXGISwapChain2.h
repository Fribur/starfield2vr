#pragma once
#include <d3d12.h>

#include "mods/D3D12CallBackManager.h"
#include "framework.h"


// IDXGISwapChain2 vTable
//[0]   QueryInterface
//[1]   AddRef
//[2]   Release
//[3]   SetPrivateData
//[4]   SetPrivateDataInterface
//[5]   GetPrivateData
//[6]   GetParent
//[7]   GetDevice
//[8]   Present
//[9]   GetBuffer
//[10]  SetFullscreenState
//[11]  GetFullscreenState
//[12]  GetDesc
//[13]  ResizeBuffers
//[14]  ResizeTarget
//[15]  GetContainingOutput
//[16]  GetFrameStatistics
//[17]  GetLastPresentCount
//[18]  GetDesc1
//[19]  GetFullscreenDesc
//[20]  GetHwnd
//[21]  GetCoreWindow
//[22]  Present1
//[23]  IsTemporaryMonoSupported
//[24]  GetRestrictToOutput
//[25]  SetBackgroundColor
//[26]  GetBackgroundColor
//[27]  SetRotation
//[28]  GetRotation
//[29]  SetSourceSize
//[30]  GetSourceSize
//[31]  SetMaximumFrameLatency
//[32]  GetMaximumFrameLatency
//[33]  GetFrameLatencyWaitableObject
//[34]  SetMatrixTransform
//[35]  GetMatrixTransform
typedef HRESULT(WINAPI* IDXGISwapChain2_Present)(IDXGISwapChain2*, UINT, UINT);
IDXGISwapChain2_Present IDXGISwapChain2_pOnPresent = nullptr; //original function pointer after hook

HRESULT WINAPI IDXGISwapChain2_detourOnPresent(IDXGISwapChain2* pSwapChain2, UINT SyncInterval, UINT Flags) {
    VRFramework::get_callback_manager()->IDXGISwapChain_onBeforePresent(pSwapChain2, SyncInterval, Flags);
    auto result = IDXGISwapChain2_pOnPresent(pSwapChain2, SyncInterval, Flags);
    if (SUCCEEDED(result)) {
        VRFramework::get_callback_manager()->IDXGISwapChain_onAfterPresent(pSwapChain2, SyncInterval, Flags);
    }
    return result;
}

typedef HRESULT(WINAPI* IDXGISwapChain2_Present1)(IDXGISwapChain2*, UINT, UINT, const DXGI_PRESENT_PARAMETERS*);
IDXGISwapChain2_Present1 IDXGISwapChain2_pOnPresent1 = nullptr; //original function pointer after hook

HRESULT WINAPI IDXGISwapChain2_detourOnPresent1(IDXGISwapChain2* pSwapChain2, UINT SyncInterval, UINT Flags, const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
    VRFramework::get_callback_manager()->IDXGISwapChain_onBeforePresent(pSwapChain2, SyncInterval, Flags);
    auto result = IDXGISwapChain2_pOnPresent1(pSwapChain2, SyncInterval, Flags, pPresentParameters);
    if (SUCCEEDED(result)) {
        VRFramework::get_callback_manager()->IDXGISwapChain_onAfterPresent(pSwapChain2, SyncInterval, Flags);
    }
    return result;
}


void IDXGISwapChain2_Hook(IDXGISwapChain2* pSwapChain2) {
    if (IDXGISwapChain2_pOnPresent == nullptr) {
        VRFramework::hook_function(pSwapChain2, 8, &IDXGISwapChain2_detourOnPresent, &IDXGISwapChain2_pOnPresent);
    }
    if (IDXGISwapChain2_pOnPresent1 == nullptr) {
        VRFramework::hook_function(pSwapChain2, 22, &IDXGISwapChain2_detourOnPresent1, &IDXGISwapChain2_pOnPresent1);
    }
}