#pragma once
#include <d3d12.h>
#include "mods/D3D12CallBackManager.h"
#include "framework.h"

// IDXGISwapChain vTable
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
typedef HRESULT(WINAPI* IDXGISwapChain_Present)(IDXGISwapChain*, UINT, UINT);
IDXGISwapChain_Present pOnPresent = nullptr; //original function pointer after hook

HRESULT WINAPI detourOnPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	VRFramework::get_callback_manager()->IDXGISwapChain_onBeforePresent(pSwapChain, SyncInterval, Flags);
	auto result = pOnPresent(pSwapChain, SyncInterval, Flags);
    if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->IDXGISwapChain_onAfterPresent(pSwapChain, SyncInterval, Flags);
	}
	return result;
}


void IDXGISwapChain_Hook(IDXGISwapChain* pSwapChain) {
    if (pOnPresent == nullptr) {
        VRFramework::hook_function(pSwapChain, 8, &detourOnPresent, &pOnPresent);
    }
}