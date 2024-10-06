#pragma once
#include <MinHook.h>
#include <d3d12.h>
#include "D3D12CommandList.h"
#include "D3D12Resource.h"
#include "winerror.h"
#include <set>
#include "ID3D12PipelineLibrary.h"
// ID3D12Device vTable indices
// 0 QueryInterface
// 1 AddRef
// 2 Release
// 3 GetPrivateData
// 4 SetPrivateData
// 5 SetPrivateDataInterface
// 6 SetName
// 7 GetNodeCount
// 8 CreateCommandQueue
// 9 CreateCommandAllocator
// 10 CreateGraphicsPipelineState
// 11 CreateComputePipelineState
// 12 CreateCommandList
// 13 CheckFeatureSupport
// 14 CreateDescriptorHeap
// 15 GetDescriptorHandleIncrementSize
// 16 CreateRootSignature
// 17 CreateConstantBufferView
// 18 CreateShaderResourceView
// 19 CreateUnorderedAccessView
// 20 CreateRenderTargetView
// 21 CreateDepthStencilView
// 22 CreateSampler
// 23 CopyDescriptors
// 24 CopyDescriptorsSimple
// 25 GetResourceAllocationInfo
// 26 GetCustomHeapProperties
// 27 CreateCommittedResource
// 28 CreateHeap
// 29 CreatePlacedResource
// 30 CreateReservedResource
// 31 CreateSharedHandle
// 32 OpenSharedHandle
// 33 OpenSharedHandleByName
// 34 MakeResident
// 35 Evict
// 36 CreateFence
// 37 GetDeviceRemovedReason
// 38 GetCopyableFootprints
// 39 CreateQueryHeap
// 40 SetStablePowerState
// 41 CreateCommandSignature
// 42 GetResourceTiling
// 43 GetAdapterLuid
// ID3D12Device1 vTable indices
// 44 CreatePipelineLibrary
// 45 SetEventOnMultipleFenceCompletion
// 46 SetResidencyPriority
// ID3D12Device2 vTable indices
// 47 CreatePipelineState
// ID3D12Device3 vTable indices
// 48 OpenExistingHeapFromAddress
// 49 OpenExistingHeapFromFileMapping
// 50 EnqueueMakeResident
// ID3D12Device4 vTable indices
// 51 CreateCommandList1
// 52 CreateProtectedResourceSession
// 53 CreateCommittedResource1
// 54 CreateHeap1
// 55 CreateReservedResource1
// 56 GetResourceAllocationInfo1
// ID3D12Device5 vTable indices
// 57 CreateLifetimeTracker
// 58 RemoveDevice
// 59 EnumerateMetaCommands
// 60 EnumerateMetaCommandParameters
// 61 CreateMetaCommand
// 62 CreateStateObject
// 63 GetRaytracingAccelerationStructurePrebuildInfo
// 64 CheckDriverMatchingIdentifier
// ID3D12Device6 vTable indices
// 65 SetBackgroundProcessingMode
// ID3D12Device7 vTable indices
// 66 AddToStateObject
// 67 CreateProtectedResourceSession1
// ID3D12Device8 vTable indices
// 68 GetResourceAllocationInfo2
// 69 CreateCommittedResource2
// 70 CreatePlacedResource1
// 71 CreateSamplerFeedbackUnorderedAccessView
// 72 GetCopyableFootprints1
// ID3D12Device9 vTable indices
// 73 CreateShaderCacheSession
// 74 ShaderCacheControl
// 75 CreateCommandQueue1
// ID3D12Device10 vTable indices
// 76 CreateCommittedResource3
// 77 CreatePlacedResource2
// 78 CreateReservedResource3
// ID3D12Device11 vTable indices

typedef HRESULT(WINAPI* ID3D12Device_createCommandList)(ID3D12Device*, UINT, D3D12_COMMAND_LIST_TYPE, ID3D12CommandAllocator*, ID3D12PipelineState*, REFIID, void**);
ID3D12Device_createCommandList pCreateCoommandList = nullptr; //original function pointer after hook

HREFTYPE WINAPI detourCreateCommandList(ID3D12Device* pDevice, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* pCommandAllocator, ID3D12PipelineState* pInitialState, REFIID riid, void** ppCommandList) {
    auto result = pCreateCoommandList(pDevice, nodeMask, type, pCommandAllocator, pInitialState, riid, ppCommandList);
    if (SUCCEEDED(result) && type == D3D12_COMMAND_LIST_TYPE_DIRECT) {
        ID3D12GraphicsCommandList* pCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(*ppCommandList);
        ID3D12GraphicsCommandList_Hook(pCommandList);
    }
    return result;
}


typedef HRESULT(WINAPI* ID3D12Device_CreateCommittedResource)(ID3D12Device*, const D3D12_HEAP_PROPERTIES*, D3D12_HEAP_FLAGS, const D3D12_RESOURCE_DESC*, D3D12_RESOURCE_STATES, const D3D12_CLEAR_VALUE*, REFIID, void**);
ID3D12Device_CreateCommittedResource pCreateCommittedResource = nullptr; //original function pointer after hook


HRESULT WINAPI detourCreateCommittedResource(ID3D12Device* pDevice, const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, REFIID riidResource, void** ppvResource) {
    auto result = pCreateCommittedResource(pDevice, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
    if (SUCCEEDED(result)) {
        ID3D12Resource* pResource = reinterpret_cast<ID3D12Resource*>(*ppvResource);
        ID3D12Resource_Hook(pResource);
        VRFramework::get_callback_manager()->ID3D12Device_CreateCommittedResource(pDevice, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
    }
    return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreatePlacedResource)(ID3D12Device*, ID3D12Heap*, UINT64, const D3D12_RESOURCE_DESC*, D3D12_RESOURCE_STATES, const D3D12_CLEAR_VALUE*, REFIID, void**);
ID3D12Device_CreatePlacedResource pCreatePlacedResource = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreatePlacedResource(ID3D12Device* pDevice, ID3D12Heap* pHeap, UINT64 HeapOffset, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, REFIID riid, void** ppvResource) {
	auto result = pCreatePlacedResource(pDevice, pHeap, HeapOffset, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);
    if (SUCCEEDED(result)) {
		ID3D12Resource* pResource = reinterpret_cast<ID3D12Resource*>(*ppvResource);
		ID3D12Resource_Hook(pResource);
		VRFramework::get_callback_manager()->ID3D12Device_onCreatePlacedResource(pDevice, pHeap, HeapOffset, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreateConstantBufferView)(ID3D12Device*, const D3D12_CONSTANT_BUFFER_VIEW_DESC*, D3D12_CPU_DESCRIPTOR_HANDLE);
ID3D12Device_CreateConstantBufferView pCreateConstantBufferView = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreateConstantBufferView(ID3D12Device* pDevice, const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) {
	auto result = pCreateConstantBufferView(pDevice, pDesc, DestDescriptor);
    if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12Device_CreateConstantBufferView(pDevice, pDesc, DestDescriptor);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreateRootSignature)(ID3D12Device*, UINT, const void*, SIZE_T, REFIID, void**);
ID3D12Device_CreateRootSignature pCreateRootSignature = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreateRootSignature(ID3D12Device* pDevice, UINT nodeMask, const void* pBlobWithRootSignature, SIZE_T blobLengthInBytes, REFIID riid, void** ppvRootSignature) {
	auto result = pCreateRootSignature(pDevice, nodeMask, pBlobWithRootSignature, blobLengthInBytes, riid, ppvRootSignature);
    if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12Device_CreateRootSignature(pDevice, nodeMask, pBlobWithRootSignature, blobLengthInBytes, riid, ppvRootSignature);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreateGraphicsPipelineState)(ID3D12Device*, const D3D12_GRAPHICS_PIPELINE_STATE_DESC*, REFIID, void**);
ID3D12Device_CreateGraphicsPipelineState pCreateGraphicsPipelineState = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreateGraphicsPipelineState(ID3D12Device* pDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid, void** ppPipelineState) {
	auto result = pCreateGraphicsPipelineState(pDevice, pDesc, riid, ppPipelineState);
    if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12Device_CreateGraphicsPipelineState(pDevice, pDesc, riid, ppPipelineState);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreatePipelineLibrary)(ID3D12Device1*, const void*, SIZE_T, REFIID, void**);
ID3D12Device_CreatePipelineLibrary pCreatePipelineLibrary = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreatePipelineLibrary(ID3D12Device1* pDevice, const void* pLibraryBlob, SIZE_T BlobLength, REFIID riid, void** ppPipelineLibrary) {
	auto result = pCreatePipelineLibrary(pDevice, pLibraryBlob, BlobLength, riid, ppPipelineLibrary);
	if (SUCCEEDED(result)) {
		ID3D12PipelineLibrary_Hook(riid, ppPipelineLibrary);
		VRFramework::get_callback_manager()->ID3D12Device_CreatePipelineLibrary(pDevice, pLibraryBlob, BlobLength, riid, ppPipelineLibrary);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12Device_CreatePipelineState)(ID3D12Device2*, const D3D12_PIPELINE_STATE_STREAM_DESC*, REFIID, void**);
ID3D12Device_CreatePipelineState pCreatePipelineState = nullptr; //original function pointer after hook

HRESULT WINAPI detourCreatePipelineState(ID3D12Device2* pDevice, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid, void** ppPipelineState) {
	auto result = pCreatePipelineState(pDevice, pDesc, riid, ppPipelineState);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12Device_CreatePipelineState(pDevice, pDesc, riid, ppPipelineState);
	}
	return result;
}

struct GUIDComparer
{
	bool operator()(const GUID& Left, const GUID& Right) const
	{
		// comparison logic goes here
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};
static const std::set<IID, GUIDComparer> iid_lookup = {
	__uuidof(ID3D12Device),
	__uuidof(ID3D12Device1),
	__uuidof(ID3D12Device2),
	__uuidof(ID3D12Device3),
	__uuidof(ID3D12Device4),
	__uuidof(ID3D12Device5),
	__uuidof(ID3D12Device6),
	__uuidof(ID3D12Device7),
	__uuidof(ID3D12Device8),
	__uuidof(ID3D12Device9),
	__uuidof(ID3D12Device10)
};


void ID3D12Device_Hook(REFIID riid, void** ppDevice) {
    auto  pos = iid_lookup.find(riid);
	int version = (pos != iid_lookup.end()) ? std::distance(iid_lookup.begin(), pos) : 0;

	{
		ID3D12Device* pDevice = reinterpret_cast<ID3D12Device*>(*ppDevice);
		if (pCreateCommittedResource == nullptr) {
			VRFramework::hook_function(pDevice, 27, &detourCreateCommittedResource, &pCreateCommittedResource);
		}
		if (pCreatePlacedResource == nullptr) {
			VRFramework::hook_function(pDevice, 29, &detourCreatePlacedResource, &pCreatePlacedResource);
		}
		if (pCreateConstantBufferView == nullptr) {
			VRFramework::hook_function(pDevice, 17, &detourCreateConstantBufferView, &pCreateConstantBufferView);
		}
		if (pCreateRootSignature == nullptr) {
			VRFramework::hook_function(pDevice, 16, &detourCreateRootSignature, &pCreateRootSignature);
		}
		if (pCreateGraphicsPipelineState == nullptr) {
			VRFramework::hook_function(pDevice, 10, &detourCreateGraphicsPipelineState, &pCreateGraphicsPipelineState);
		}
		if (pCreateCoommandList == nullptr) {
			VRFramework::hook_function(pDevice, 12, &detourCreateCommandList, &pCreateCoommandList);
		}
	}
	if(version > 0) {
		ID3D12Device1* pDevice = reinterpret_cast<ID3D12Device1*>(*ppDevice);
		if (pCreatePipelineLibrary == nullptr) {
			VRFramework::hook_function(pDevice, 44, &detourCreatePipelineLibrary, &pCreatePipelineLibrary);
		}
	}

	if (version > 1) {
		ID3D12Device2* pDevice = reinterpret_cast<ID3D12Device2*>(*ppDevice);
		if (pCreatePipelineState == nullptr) {
			VRFramework::hook_function(pDevice, 47, &detourCreatePipelineState, &pCreatePipelineState);
		}
	}

}