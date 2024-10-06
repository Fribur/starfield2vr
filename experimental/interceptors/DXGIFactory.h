#pragma once
#include "DXGISwapChain.h"
#include "DXGISwapChain1.h"
#include "DXGISwapChain2.h"
#include <Dxgi1_3.h>
#include <MinHook.h>
#include <d3d12.h>
#include <dxgi.h>

//[0]   QueryInterface
//[1]   AddRef
//[2]   Release
//[3]   SetPrivateData
//[4]   SetPrivateDataInterface
//[5]   GetPrivateData
//[6]   GetParent
//[7]   EnumAdapters
//[8]   MakeWindowAssociation
//[9]   GetWindowAssociation
//[10]  CreateSwapChain
//[11]  CreateSoftwareAdapter
typedef HRESULT(WINAPI* IDXGIFactory_CreateSwapChain)(IDXGIFactory*, IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
IDXGIFactory_CreateSwapChain pCreateSwapChain = nullptr; // original function pointer after hook

HRESULT WINAPI detourCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
    auto result = pCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
    if (SUCCEEDED(result)) {
        IDXGISwapChain* pSwapChain = reinterpret_cast<IDXGISwapChain*>(*ppSwapChain);
        IDXGISwapChain_Hook(pSwapChain);
    }
    return result;
}

// IDXGIFactory1 vTable indexes
//[0]   QueryInterface
//[1]   AddRef
//[2]   Release
//[3]   SetPrivateData
//[4]   SetPrivateDataInterface
//[5]   GetPrivateData
//[6]   GetParent
//[7]   EnumAdapters
//[8]   MakeWindowAssociation
//[9]   GetWindowAssociation
//[10]  CreateSwapChain
//[11]  CreateSoftwareAdapter
//[12]  EnumAdapters1
//[13]  IsCurrent
typedef HRESULT(WINAPI* IDXGIFactory1_CreateSwapChain)(IDXGIFactory1*, IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
IDXGIFactory1_CreateSwapChain pCreateSwapChain1 = nullptr; // original function pointer after hook

HRESULT WINAPI detourCreateSwapChain1(IDXGIFactory1* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
    auto result = pCreateSwapChain1(pFactory, pDevice, pDesc, ppSwapChain);
    if (SUCCEEDED(result)) {
        IDXGISwapChain1* pSwapChain = reinterpret_cast<IDXGISwapChain1*>(*ppSwapChain);
        IDXGISwapChain1_Hook(pSwapChain);
    }
    return result;
}

// IDXGIFactory2 vTable indexes
//[0]   QueryInterface
//[1]   AddRef
//[2]   Release
//[3]   SetPrivateData
//[4]   SetPrivateDataInterface
//[5]   GetPrivateData
//[6]   GetParent
//[7]   EnumAdapters
//[8]   MakeWindowAssociation
//[9]   GetWindowAssociation
//[10]  CreateSwapChain
//[11]  CreateSoftwareAdapter
//[12]  EnumAdapters1
//[13]  IsCurrent
//[14]  IsWindowedStereoEnabled
//[15]  CreateSwapChainForHwnd
//[16]  CreateSwapChainForCoreWindow
//[17]  GetSharedResourceAdapterLuid
//[18]  RegisterStereoStatusWindow
//[19]  RegisterStereoStatusEvent
//[20]  UnregisterStereoStatus
//[21]  RegisterOcclusionStatusWindow
//[22]  RegisterOcclusionStatusEvent
//[23]  UnregisterOcclusionStatus
//[24]  CreateSwapChainForComposition
typedef HRESULT(WINAPI* IDXGIFactory2_CreateSwapChain)(IDXGIFactory2*, IUnknown*, DXGI_SWAP_CHAIN_DESC1*, IDXGISwapChain1**);
IDXGIFactory2_CreateSwapChain pCreateSwapChain2 = nullptr; // original function pointer after hook

HRESULT WINAPI detourCreateSwapChain2(IDXGIFactory2* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGISwapChain1** ppSwapChain)
{
    VRFramework::get_callback_manager()->IDXGIFactory2_onBeforeCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
    auto result = pCreateSwapChain2(pFactory, pDevice, pDesc, ppSwapChain);
    if (SUCCEEDED(result)) {
        IDXGISwapChain1* pSwapChain = reinterpret_cast<IDXGISwapChain1*>(*ppSwapChain);
        IDXGISwapChain1_Hook(pSwapChain);
    }
    return result;
}

typedef HRESULT(STDMETHODCALLTYPE* IDXGIFactory2_CreateSwapChainForHwnd)(IDXGIFactory2*, IUnknown*, HWND, DXGI_SWAP_CHAIN_DESC*, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC*,
                                                                         IDXGIOutput*, IDXGISwapChain1**);
IDXGIFactory2_CreateSwapChainForHwnd pCreateSwapChainForHwnd = nullptr; // original function pointer after hook

HRESULT WINAPI detourCreateSwapChainForHwnd(IDXGIFactory2* pFactory, IUnknown* pDevice, HWND hWnd, DXGI_SWAP_CHAIN_DESC* pDesc,
                                            const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
    VRFramework::get_callback_manager()->IDXGIFactory2_onBeforeCreateSwapChainForHwnd(pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
    auto result = pCreateSwapChainForHwnd(pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
    if (SUCCEEDED(result)) {
        IDXGISwapChain1* pSwapChain = reinterpret_cast<IDXGISwapChain1*>(*ppSwapChain);
        IDXGISwapChain1_Hook(pSwapChain);
    }
    return result;
}

typedef HRESULT(STDMETHODCALLTYPE* IDXGIFactory2_CreateSwapChainForCoreWindow)(IDXGIFactory2*, IUnknown*, IUnknown*, const DXGI_SWAP_CHAIN_DESC1*, IDXGIOutput*, IDXGISwapChain1**);
IDXGIFactory2_CreateSwapChainForCoreWindow pCreateSwapChainForCoreWindow = nullptr; // original function pointer after hook

HRESULT WINAPI detourCreateSwapChainForCoreWindow(IDXGIFactory2* pFactory, IUnknown* pDevice, IUnknown* pWindow, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
                                                  IDXGISwapChain1** ppSwapChain)
{
    auto result = pCreateSwapChainForCoreWindow(pFactory, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
    if (SUCCEEDED(result)) {
        IDXGISwapChain1* pSwapChain = reinterpret_cast<IDXGISwapChain1*>(*ppSwapChain);
        IDXGISwapChain1_Hook(pSwapChain);
    }
    return result;
}

void IDXGIFactory_Hook(IDXGIFactory* pFactory)
{
    if (pCreateSwapChain == nullptr) {
        VRFramework::hook_function(pFactory, 10, &detourCreateSwapChain, &pCreateSwapChain);
    }
}

void IDXGIFactory1_Hook(IDXGIFactory1* pFactory1)
{
    if (pCreateSwapChain1 == nullptr) {
        VRFramework::hook_function(pFactory1, 10, &detourCreateSwapChain1, &pCreateSwapChain1);
    }
}

void IDXGIFactory2_Hook(IDXGIFactory2* pFactory2)
{
    if (pCreateSwapChain2 == nullptr) {
        VRFramework::hook_function(pFactory2, 10, &detourCreateSwapChain2, &pCreateSwapChain2);
    }
    if (pCreateSwapChainForHwnd == nullptr) {
        VRFramework::hook_function(pFactory2, 15, &detourCreateSwapChainForHwnd, &pCreateSwapChainForHwnd);
    }
    if (pCreateSwapChainForCoreWindow == nullptr) {
        VRFramework::hook_function(pFactory2, 16, &detourCreateSwapChainForCoreWindow, &pCreateSwapChainForCoreWindow);
    }
}
