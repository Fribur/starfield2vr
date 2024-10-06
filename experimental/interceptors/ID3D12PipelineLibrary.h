#pragma once
#include <d3d12.h>
#include "framework.h"
// vtable ID3D12PipelineLibrary
// [0]: QueryInterface
// [1]: AddRef
// [2]: Release
// [3]: GetPrivateData
// [4]: SetPrivateData
// [5]: SetPrivateDataInterface
// [6]: SetName
// [7]: GetDevice
// [8]: StorePipeline
// [9]: LoadGraphicsPipeline
// [10]: LoadComputePipeline
// [11]: GetSerializedSize
// [12]: Serialize
// ID3D12PipelineLibrary1 vtable
// [13]: LoadPipeline


typedef HRESULT(WINAPI* ID3D12PipelineLibrary_LoadGraphicsPipeline)(ID3D12PipelineLibrary* pPipelineLibrary, LPCWSTR pName, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid, void** ppPipelineState);
ID3D12PipelineLibrary_LoadGraphicsPipeline oID3D12PipelineLibrary_LoadGraphicsPipeline = nullptr;

HRESULT WINAPI hID3D12PipelineLibrary_LoadGraphicsPipeline(ID3D12PipelineLibrary* pPipelineLibrary, LPCWSTR pName, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid, void** ppPipelineState)
{
	HRESULT result = oID3D12PipelineLibrary_LoadGraphicsPipeline(pPipelineLibrary, pName, pDesc, riid, ppPipelineState);
	if (SUCCEEDED(result))
	{
		VRFramework::get_callback_manager()->onID3D12PipelineLibrary_LoadGraphicsPipeline(pPipelineLibrary, pName, pDesc, riid, ppPipelineState);
	}
	return result;
}

typedef HRESULT(WINAPI* ID3D12PipelineLibrary_LoadPipeline)(ID3D12PipelineLibrary1* pPipelineLibrary, LPCWSTR pName, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid, void** ppPipelineState);
ID3D12PipelineLibrary_LoadPipeline oID3D12PipelineLibrary_LoadPipeline = nullptr;

HRESULT WINAPI hID3D12PipelineLibrary_LoadPipeline(ID3D12PipelineLibrary1* pPipelineLibrary, LPCWSTR pName, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid, void** ppPipelineState)
{
	HRESULT result = oID3D12PipelineLibrary_LoadPipeline(pPipelineLibrary, pName, pDesc, riid, ppPipelineState);
	if (SUCCEEDED(result))
	{
		VRFramework::get_callback_manager()->onID3D12PipelineLibrary_LoadPipeline(pPipelineLibrary, pName, pDesc, riid, ppPipelineState);
	}
	return result;
}

void ID3D12PipelineLibrary_Hook(REFIID riid, void** ppPipelineLibrary)
{
	{
		ID3D12PipelineLibrary* pPipelineLibrary = reinterpret_cast<ID3D12PipelineLibrary*>(*ppPipelineLibrary);
		if (oID3D12PipelineLibrary_LoadGraphicsPipeline == nullptr)
		{
			VRFramework::hook_function(pPipelineLibrary, 9, &hID3D12PipelineLibrary_LoadGraphicsPipeline, &oID3D12PipelineLibrary_LoadGraphicsPipeline);
		}
	}
	if (__uuidof(ID3D12PipelineLibrary1) == riid)
	{
		ID3D12PipelineLibrary1* pPipelineLibrary = reinterpret_cast<ID3D12PipelineLibrary1*>(*ppPipelineLibrary);
		if (oID3D12PipelineLibrary_LoadPipeline == nullptr)
		{
			VRFramework::hook_function(pPipelineLibrary, 13, &hID3D12PipelineLibrary_LoadPipeline, &oID3D12PipelineLibrary_LoadPipeline);
		}
	}

}