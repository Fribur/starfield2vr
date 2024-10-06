#pragma once
#include <d3d12.h>
#include "mods/D3D12CallBackManager.h"
#include "framework.h"

// IDXGISwapChain1 vTable
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
typedef HRESULT(WINAPI* IDXGISwapChain1_Present)(IDXGISwapChain1*, UINT, UINT);
IDXGISwapChain1_Present IDXGISwapChain1_pOnPresent = nullptr; //original function pointer after hook

HRESULT WINAPI IDXGISwapChain1_detourOnPresent(IDXGISwapChain1* pSwapChain1, UINT SyncInterval, UINT Flags) {
   VRFramework::get_callback_manager()->IDXGISwapChain_onBeforePresent(pSwapChain1, SyncInterval, Flags);
	auto result = IDXGISwapChain1_pOnPresent(pSwapChain1, SyncInterval, Flags);
    if (SUCCEEDED(result)) {
       VRFramework::get_callback_manager()->IDXGISwapChain_onAfterPresent(pSwapChain1, SyncInterval, Flags);
   	}
	return result;
}

typedef HRESULT(WINAPI* IDXGISwapChain1_Present1)(IDXGISwapChain1*, UINT, UINT, const DXGI_PRESENT_PARAMETERS*);
IDXGISwapChain1_Present1 IDXGISwapChain1_pOnPresent1 = nullptr; //original function pointer after hook

HRESULT WINAPI IDXGISwapChain1_detourOnPresent1(IDXGISwapChain1* pSwapChain1, UINT SyncInterval, UINT Flags, const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
    VRFramework::get_callback_manager()->IDXGISwapChain_onBeforePresent(pSwapChain1, SyncInterval, Flags);
	auto result = IDXGISwapChain1_pOnPresent1(pSwapChain1, SyncInterval, Flags, pPresentParameters);
    if (SUCCEEDED(result)) {
        VRFramework::get_callback_manager()->IDXGISwapChain_onAfterPresent(pSwapChain1, SyncInterval, Flags);
   	}
	return result;
}


void IDXGISwapChain1_Hook(IDXGISwapChain1* pSwapChain1) {
    if (IDXGISwapChain1_pOnPresent == nullptr) {
        VRFramework::hook_function(pSwapChain1, 8, &IDXGISwapChain1_detourOnPresent, &IDXGISwapChain1_pOnPresent);
    }
    if (IDXGISwapChain1_pOnPresent1 == nullptr) {
		VRFramework::hook_function(pSwapChain1, 22, &IDXGISwapChain1_detourOnPresent1, &IDXGISwapChain1_pOnPresent1);
	}
}