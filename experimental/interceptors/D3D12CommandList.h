#pragma once
#include <d3d12.h>
#include "framework.h"
// vtable ID3D12GraphicsCommandList
// [0]: QueryInterface
// [1]: AddRef
// [2]: Release
// [3]: GetPrivateData
// [4]: SetPrivateData
// [5]: SetPrivateDataInterface
// [6]: SetName
// [7]: GetDevice
// [8]: GetType
// [9]: Close
// [10]: Reset
// [11]: ClearState
// [12]: DrawInstanced
// [13]: DrawIndexedInstanced
// [14]: Dispatch
// [15]: CopyBufferRegion
// [16]: CopyTextureRegion
// [17]: CopyResource
// [18]: CopyTiles
// [19]: ResolveSubresource
// [20]: IASetPrimitiveTopology
// [21]: RSSetViewports
// [22]: RSSetScissorRects
// [23]: OMSetBlendFactor
// [24]: OMSetStencilRef
// [25]: SetPipelineState
// [26]: ResourceBarrier
// [27]: ExecuteBundle
// [28]: SetDescriptorHeaps
// [29]: SetComputeRootSignature
// [30]: SetGraphicsRootSignature
// [31]: SetComputeRootDescriptorTable
// [32]: SetGraphicsRootDescriptorTable
// [33]: SetComputeRoot32BitConstant
// [34]: SetGraphicsRoot32BitConstant
// [35]: SetComputeRoot32BitConstants
// [36]: SetGraphicsRoot32BitConstants
// [37]: SetComputeRootConstantBufferView
// [38]: SetGraphicsRootConstantBufferView
// [39]: SetComputeRootShaderResourceView
// [40]: SetGraphicsRootShaderResourceView
// [41]: SetComputeRootUnorderedAccessView
// [42]: SetGraphicsRootUnorderedAccessView
// [43]: IASetIndexBuffer
// [44]: IASetVertexBuffers
// [45]: SOSetTargets
// [46]: OMSetRenderTargets
// [47]: ClearDepthStencilView
// [48]: ClearRenderTargetView
// [49]: ClearUnorderedAccessViewUint
// [50]: ClearUnorderedAccessViewFloat
// [51]: DiscardResource
// [52]: BeginQuery
// [53]: EndQuery
// [54]: ResolveQueryData
// [55]: SetPredication
// [56]: SetMarker
// [57]: BeginEvent
// [58]: EndEvent
// [59]: ExecuteIndirect
typedef HRESULT(WINAPI* setGraphicsRootConstantBufferView)(ID3D12GraphicsCommandList*, UINT, D3D12_GPU_VIRTUAL_ADDRESS);
setGraphicsRootConstantBufferView pSetGraphicsRootConstantBufferView = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetGraphicsRootConstantBufferView(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) {
	auto result = pSetGraphicsRootConstantBufferView(pCommandList, RootParameterIndex, BufferLocation);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetGraphicsRootConstantBufferView(pCommandList, RootParameterIndex, BufferLocation);
	}
	return result;
}

typedef HRESULT(WINAPI* setSetGraphicsRoot32BitConstants)(ID3D12GraphicsCommandList*, UINT, UINT, const void*, UINT);
setSetGraphicsRoot32BitConstants pSetSetGraphicsRoot32BitConstants = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetGraphicsRoot32BitConstants(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex, UINT Num32BitValuesToSet, const void* pSrcData, UINT DestOffsetIn32BitValues) {
	auto result = pSetSetGraphicsRoot32BitConstants(pCommandList, RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetGraphicsRoot32BitConstants(pCommandList, RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12GraphicsCommandList_SetGraphicsRootSignature)(ID3D12GraphicsCommandList*, ID3D12RootSignature*);
ID3D12GraphicsCommandList_SetGraphicsRootSignature pSetGraphicsRootSignature = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetGraphicsRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature) {
	auto result = pSetGraphicsRootSignature(pCommandList, pRootSignature);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetGraphicsRootSignature(pCommandList, pRootSignature);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12GraphicsCommandList_SetCoputeRootSignature)(ID3D12GraphicsCommandList*, ID3D12RootSignature*);
ID3D12GraphicsCommandList_SetCoputeRootSignature pSetComputeRootSignature = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetComputeRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature) {
	auto result = pSetComputeRootSignature(pCommandList, pRootSignature);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetComputeRootSignature(pCommandList, pRootSignature);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12GraphicsCommandList_SetPipelineState)(ID3D12GraphicsCommandList*, ID3D12PipelineState*);
ID3D12GraphicsCommandList_SetPipelineState pSetPipelineState = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetPipelineState(ID3D12GraphicsCommandList* pCommandList, ID3D12PipelineState* pPipelineState) {
	auto result = pSetPipelineState(pCommandList, pPipelineState);
	if (SUCCEEDED(result)) {
		VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetPipelineState(pCommandList, pPipelineState);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12GraphicsCommandList_RSSetViewports)(ID3D12GraphicsCommandList*, UINT, const D3D12_VIEWPORT*);
ID3D12GraphicsCommandList_RSSetViewports pRSSetViewports = nullptr; //original function pointer after hook

HRESULT WINAPI detourRSSetViewports(ID3D12GraphicsCommandList* pCommandList, UINT NumViewports, D3D12_VIEWPORT* pViewports) {
    VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onRSSetViewports(pCommandList, NumViewports, pViewports);
    return  pRSSetViewports(pCommandList, NumViewports, pViewports);
}

typedef HRESULT(WINAPI* ID3D12GraphicsCommandList_SetScissorRects)(ID3D12GraphicsCommandList*, UINT, const D3D12_RECT*);
ID3D12GraphicsCommandList_SetScissorRects pSetScissorRects = nullptr; //original function pointer after hook

HRESULT WINAPI detourSetScissorRects(ID3D12GraphicsCommandList* pCommandList, UINT NumRects, const D3D12_RECT* pRects) {
    VRFramework::get_callback_manager()->ID3D12GraphicsCommandList_onSetScissorRects(pCommandList, NumRects, pRects);
    return  pSetScissorRects(pCommandList, NumRects, pRects);
}

void ID3D12GraphicsCommandList_Hook(ID3D12GraphicsCommandList* pCommandList) {
	if (pSetGraphicsRootConstantBufferView == nullptr) {
		VRFramework::hook_function(pCommandList, 38, &detourSetGraphicsRootConstantBufferView, &pSetGraphicsRootConstantBufferView);
	}
	if (pSetSetGraphicsRoot32BitConstants == nullptr) {
		VRFramework::hook_function(pCommandList, 36, &detourSetGraphicsRoot32BitConstants, &pSetSetGraphicsRoot32BitConstants);
	}

	if (pSetGraphicsRootSignature == nullptr) {
		VRFramework::hook_function(pCommandList, 30, &detourSetGraphicsRootSignature, &pSetGraphicsRootSignature);
	}

	if (pSetPipelineState == nullptr) {
		VRFramework::hook_function(pCommandList, 25, &detourSetPipelineState, &pSetPipelineState);
	}

	if (pSetComputeRootSignature == nullptr) {
		VRFramework::hook_function(pCommandList, 29, &detourSetComputeRootSignature, &pSetComputeRootSignature);
	}

  if (pRSSetViewports == nullptr) {
    VRFramework::hook_function(pCommandList, 21, &detourRSSetViewports, &pRSSetViewports);
  }

  if (pSetScissorRects == nullptr) {
    VRFramework::hook_function(pCommandList, 22, &detourSetScissorRects, &pSetScissorRects);
  }
}