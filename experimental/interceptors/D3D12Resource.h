#pragma once
#include <d3d12.h>
#include "framework.h"

// ID3D12Resource vTable indexes
// 0 QueryInterface
// 1 AddRef
// 2 Release
// 3 GetPrivateData
// 4 SetPrivateData
// 5 SetPrivateDataInterface
// 6 SetName
// 7 GetDevice
// 8 Map
// 9 Unmap
// 10 WriteToSubresource
// 11 ReadFromSubresource
// 12 GetHeapProperties
// 13 GetDesc
// 14 GetGPUVirtualAddress
typedef HRESULT(WINAPI* mapResource)(ID3D12Resource*, UINT, const D3D12_RANGE*, void**);
mapResource pMapResource = nullptr; //original function pointer after hook

HRESULT WINAPI detourMapResource(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pReadRange, void** ppData) {
    auto result = pMapResource(pResource, Subresource, pReadRange, ppData);
    if (SUCCEEDED(result)) {
        VRFramework::get_callback_manager()->ID3D12Resource_onMap(pResource, Subresource, pReadRange, ppData);
    }
    return result;
}

typedef HRESULT(WINAPI* unmapResource)(ID3D12Resource*, UINT, const D3D12_RANGE*);
unmapResource pUnmapResource = nullptr; //original function pointer after hook

HRESULT WINAPI detourUnmapResource(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pWrittenRange) {
	auto result = pUnmapResource(pResource, Subresource, pWrittenRange);
    if (SUCCEEDED(result)) {
        VRFramework::get_callback_manager()->ID3D12Resource_onUnmap(pResource, Subresource, pWrittenRange);
	}
	return result;
}

void ID3D12Resource_Hook(ID3D12Resource* pResource) {
    if (pMapResource == nullptr) {
        VRFramework::hook_function(pResource, 8, &detourMapResource, &pMapResource);
    }

    if (pUnmapResource == nullptr) {
        VRFramework::hook_function(pResource, 9, &detourUnmapResource, &pUnmapResource);
    }
}