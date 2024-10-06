#include "D3D12CallBackManager.h"
#include "VR.hpp"
#include "d3d12.h"
#include "framework.h"
#include <d3d12sdklayers.h>
// #include "dll_assembler.hpp"
// #include "d3d12_utils.h"
// #include <crc32_hash.hpp>
#include "hooks/CreationEngineHook.h"

D3D12CallBackManager* g_d3d12_callback_manager{};

D3D12CallBackManager::D3D12CallBackManager(HINSTANCE hModule) : hModule{ hModule }, cpuHandlesMap{} {
}

D3D12CallBackManager::~D3D12CallBackManager() {}

bool D3D12CallBackManager::XInput1_4_onGetState(UINT dwUserIndex, XINPUT_STATE* pState)
{
    return false;
}

bool D3D12CallBackManager::IDXGISwapChain_onBeforePresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    return true;
}

bool D3D12CallBackManager::IDXGISwapChain_onAfterPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    //    auto worldCamera = RE::Main::WorldCamera();
    //    worldCamera->local.translate.x = 0.2f*separation;
    //    separation = -separation;
    if (do_capture) {
        spdlog::info("--- End Frame ---");
        do_capture = false;
    }
    else {
        // The keyboard shortcut to trigger logging
        if (GetAsyncKeyState(VK_F10) & 1) {
            do_capture = true;
            spdlog::info("--- Frame ---");
        }
    }
    frameCount++;
//    dummy_counter = 0;
    spdlog::info("--- End Frame ---");
    //CreationEngineHook::Get()->m_multiCameraViewModule->SwapCameras(frameCount);
    return true;
}
/*

void D3D12CallBackManager::onUpdateProjectionMatrix(RE::NiPoint3A* translate, RE::NiMatrix3* rotate, RE::NiFrustum* pFrustum, char orpho, float* matrix)
{
    if (do_capture) {
        spdlog::info("updateProjectionMatrix2 called Frustum[l{:.8f},r{:.8f},t{:.8f},b{:.8f},n{:.8f},f{:.8f}] orpho={}", ((float*)pFrustum)[0], ((float*)pFrustum)[1],
                     ((float*)pFrustum)[2], ((float*)pFrustum)[3], ((float*)pFrustum)[4], ((float*)pFrustum)[5], orpho);
    }
}
*/

bool D3D12CallBackManager::ID3D12Resource_onMap(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pReadRange, void** ppData)
{
    auto desc = pResource->GetDesc();
    // spdlog::info("Mapped resource at {:p} with {} bytes", fmt::ptr(*ppData), desc.Width);
    return true;
}

bool D3D12CallBackManager::ID3D12Resource_onUnmap(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pWrittenRange)
{
    auto desc = pResource->GetDesc();
    // spdlog::info("Unmapped resource at 0x{:x} with {} bytes", pResource->GetGPUVirtualAddress(), desc.Width);
    return true;
}

bool D3D12CallBackManager::onMemcpy(void* dst, const void* src, size_t size)
{
    // if (size < 512 && size % 4 == 0) {
    //	float* ptr;
    //	ptr = (float*)src;
    //	// to stirng array from ptr size of size/4
    //	std::string str = "[";
    //	for (int i = 0; i < size/4; i++) {
    //		str += std::to_string(ptr[i]) + ", ";
    //	}
    //	str += "]";
    //	spdlog::info("Memcpy: {} bytes from {:p} to {:p} {}", size, fmt::ptr(src), fmt::ptr(dst), str);
    // }
    // else {
    // spdlog::info("Memcpy: {} bytes from {:p} to {:p}", size, fmt::ptr(src), fmt::ptr(dst));

    //}
    return true;
}

bool D3D12CallBackManager::ID3D12GraphicsCommandList_onSetGraphicsRootConstantBufferView(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex,
                                                                                         D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
    if (do_capture) {
        // spdlog::info("SetGraphicsRootConstantBufferView: index {} at 0x{:x}", RootParameterIndex, BufferLocation);
    }
    return true;
}

bool D3D12CallBackManager::ID3D12Device_CreateConstantBufferView(ID3D12Device* pDevice, const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
{
    // spdlog::info("CreateConstantBufferView: index {} at {:p}", *pDesc, fmt::ptr(&DestDescriptor.ptr));
    return true;
}

bool D3D12CallBackManager::ID3D12Device_CreateRootSignature(ID3D12Device* pDevice, UINT nodeMask, const void* pBlobWithRootSignature, SIZE_T blobLengthInBytes, REFIID riid,
                                                            void** ppvRootSignature)
{
    //	ID3D12RootSignatureDeserializer* pDeserializer = nullptr;
    //
    //	PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER D3D12CreateRootSignatureDeserializer_f = (PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER)GetProcAddress(g_d3d12,
    //"D3D12CreateRootSignatureDeserializer"); 	HRESULT hr = D3D12CreateRootSignatureDeserializer_f(pBlobWithRootSignature, blobLengthInBytes, IID_PPV_ARGS(&pDeserializer));
    //
    //	if (SUCCEEDED(hr))
    //	{
    //		const D3D12_ROOT_SIGNATURE_DESC* pDesc = pDeserializer->GetRootSignatureDesc();
    //		std::string rootParamsStr = "";
    //		for (int i = 0; i < pDesc->NumParameters; i++)
    //		{
    //			const D3D12_ROOT_PARAMETER& param = pDesc->pParameters[i];
    //			rootParamsStr += fmt::format("[{}]{}\n", i, param);
    //		}
    //		spdlog::info("Root Signature[{:p}]: {}", fmt::ptr(*ppvRootSignature), rootParamsStr);
    //		pDeserializer->Release();
    //	}
    return true;
}

bool D3D12CallBackManager::ID3D12Device_CreateGraphicsPipelineState(ID3D12Device* pDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid, void** ppPipelineState)
{
    //	if (pDesc->VS.BytecodeLength > 0) {
    //		dumpShader(L"vs", pDesc->VS.pShaderBytecode, pDesc->VS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->PS.BytecodeLength > 0) {
    //		dumpShader(L"ps", pDesc->PS.pShaderBytecode, pDesc->PS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->DS.BytecodeLength > 0) {
    //		dumpShader(L"ds", pDesc->DS.pShaderBytecode, pDesc->DS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->HS.BytecodeLength > 0) {
    //		dumpShader(L"hs", pDesc->HS.pShaderBytecode, pDesc->HS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->GS.BytecodeLength > 0) {
    //		dumpShader(L"gs", pDesc->GS.pShaderBytecode, pDesc->GS.BytecodeLength, false, 0);
    //	}
    return true;
}

bool D3D12CallBackManager::onID3D12PipelineLibrary_LoadGraphicsPipeline(ID3D12PipelineLibrary* pPipelineLibrary, LPCWSTR pName, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc,
                                                                        REFIID riid, void** ppPipelineState)
{
    //	if (pDesc->VS.BytecodeLength > 0) {
    //		dumpShader(L"vs", pDesc->VS.pShaderBytecode, pDesc->VS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->PS.BytecodeLength > 0) {
    //		dumpShader(L"ps", pDesc->PS.pShaderBytecode, pDesc->PS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->DS.BytecodeLength > 0) {
    //		dumpShader(L"ds", pDesc->DS.pShaderBytecode, pDesc->DS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->HS.BytecodeLength > 0) {
    //		dumpShader(L"hs", pDesc->HS.pShaderBytecode, pDesc->HS.BytecodeLength, false, 0);
    //	}
    //	if (pDesc->GS.BytecodeLength > 0) {
    //		dumpShader(L"gs", pDesc->GS.pShaderBytecode, pDesc->GS.BytecodeLength, false, 0);
    //	}
    return true;
}

bool D3D12CallBackManager::onID3D12PipelineLibrary_LoadPipeline(ID3D12PipelineLibrary1* pPipelineLibrary, LPCWSTR pName, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid,
                                                                void** ppPipelineState)
{
    //	ID3D12PipelineState* pPipelineStateObject = reinterpret_cast<ID3D12PipelineState*>(*ppPipelineState);
    //	PSOPrivateData privateData = PSOPrivateData();
    //	parse_pipline_stream(pDesc, &privateData);
    //	pPipelineStateObject->SetPrivateData(__uuidof(PSOPrivateData), sizeof(PSOPrivateData), &privateData);
    return true;
}

bool D3D12CallBackManager::ID3D12Device_CreatePipelineLibrary(ID3D12Device1* pDevice, const void* pLibraryBlob, SIZE_T BlobLength, REFIID riid, void** ppPipelineLibrary)
{
    return true;
}

// void D3D12CallBackManager::parse_pipline_stream(const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, PSOPrivateData* privateData)
//{
//	for (uintptr_t p = reinterpret_cast<uintptr_t>(pDesc->pPipelineStateSubobjectStream); p < (reinterpret_cast<uintptr_t>(pDesc->pPipelineStateSubobjectStream) +
//pDesc->SizeInBytes);)
//	{
//		switch (*reinterpret_cast<const D3D12_PIPELINE_STATE_SUBOBJECT_TYPE*>(p))
//		{
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS:
//			auto vs_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_VS*>(p)->data;
//			privateData->vs_crc = compute_crc32((UINT8*)vs_code.pShaderBytecode, vs_code.BytecodeLength);
//			dumpShader(L"vs", vs_code.pShaderBytecode, vs_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_VS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS:
//			auto ps_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_PS*>(p)->data;
//			privateData->ps_crc = compute_crc32((UINT8*)ps_code.pShaderBytecode, ps_code.BytecodeLength);
//			dumpShader(L"ps", ps_code.pShaderBytecode, ps_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_PS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS:
//			auto ds_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_DS*>(p)->data;
//			dumpShader(L"ds", ds_code.pShaderBytecode, ds_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_DS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS:
//			auto hs_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_HS*>(p)->data;
//			dumpShader(L"hs", hs_code.pShaderBytecode, hs_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_HS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS:
//			auto gs_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_GS*>(p)->data;
//			dumpShader(L"gs", gs_code.pShaderBytecode, gs_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_GS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS:
//			auto cs_code = reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_CS*>(p)->data;
//			privateData->cs_crc = compute_crc32((UINT8*)cs_code.pShaderBytecode, cs_code.BytecodeLength);
//			dumpShader(L"cs", cs_code.pShaderBytecode, cs_code.BytecodeLength, false, 0);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_CS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_STREAM_OUTPUT);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_BLEND_DESC);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_SAMPLE_MASK);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_RASTERIZER);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_DEPTH_STENCIL);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT:
//			//input_layout = reshade::d3d12::convert_input_layout_desc(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_INPUT_LAYOUT*>(p)->data.NumElements, reinterpret_cast<const
//D3D12_PIPELINE_STATE_STREAM_INPUT_LAYOUT*>(p)->data.pInputElementDescs); 			p += sizeof(D3D12_PIPELINE_STATE_STREAM_INPUT_LAYOUT); 			continue; 		case
//D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE: 			p += sizeof(D3D12_PIPELINE_STATE_STREAM_IB_STRIP_CUT_VALUE); 			continue; 		case
//D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY: 			p += sizeof(D3D12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY); 			continue; 		case
//D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS:
//			/*assert(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS*>(p)->data.NumRenderTargets <= 8);
//			for (UINT i = 0; i < reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS*>(p)->data.NumRenderTargets; ++i)
//			render_target_formats[i] = reshade::d3d12::convert_format(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS*>(p)->data.RTFormats[i]);
//			subobjects.push_back({ reshade::api::pipeline_subobject_type::render_target_formats, reinterpret_cast<const
//D3D12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS*>(p)->data.NumRenderTargets, render_target_formats });
//			*/
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_SAMPLE_DESC);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_NODE_MASK);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_CACHED_PSO);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_FLAGS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1:
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING:
//			assert(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_VIEW_INSTANCING*>(p)->data.ViewInstanceCount <= D3D12_MAX_VIEW_INSTANCE_COUNT);
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_VIEW_INSTANCING);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_AS:
//			assert(false); // Not implemented
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_AS);
//			continue;
//		case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS:
//			assert(false); // Not implemented
//			p += sizeof(D3D12_PIPELINE_STATE_STREAM_MS);
//			continue;
//			//case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL2:
//			//	depth_stencil_desc = reshade::d3d12::convert_depth_stencil_desc(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_DEPTH_STENCIL2*>(p)->data);
//			//	subobjects.push_back({ reshade::api::pipeline_subobject_type::depth_stencil_state, 1, &depth_stencil_desc });
//			//	p += sizeof(D3D12_PIPELINE_STATE_STREAM_DEPTH_STENCIL2);
//			//	continue;
//			//case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER1:
//			//	rasterizer_desc = reshade::d3d12::convert_rasterizer_desc(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_RASTERIZER1*>(p)->data);
//			//	subobjects.push_back({ reshade::api::pipeline_subobject_type::rasterizer_state, 1, &rasterizer_desc });
//			//	p += sizeof(D3D12_PIPELINE_STATE_STREAM_RASTERIZER1);
//			//	continue;
//			//case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER2:
//			//	rasterizer_desc = reshade::d3d12::convert_rasterizer_desc(reinterpret_cast<const D3D12_PIPELINE_STATE_STREAM_RASTERIZER2*>(p)->data);
//			//	subobjects.push_back({ reshade::api::pipeline_subobject_type::rasterizer_state, 1, &rasterizer_desc });
//			//	p += sizeof(D3D12_PIPELINE_STATE_STREAM_RASTERIZER1);
//			//	continue;
//		default:
//			// Unknown sub-object type, break out of the loop
//			assert(false);
//		}
//		break;
//	}
// }

bool D3D12CallBackManager::ID3D12Device_CreatePipelineState(ID3D12Device* pDevice, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid, void** ppPipelineState)
{
    //	ID3D12PipelineState* pPipelineStateObject = reinterpret_cast<ID3D12PipelineState*>(*ppPipelineState);
    //	PSOPrivateData privateData = PSOPrivateData();
    //	parse_pipline_stream(pDesc, &privateData);
    //	pPipelineStateObject->SetPrivateData(__uuidof(PSOPrivateData), sizeof(PSOPrivateData), &privateData);
    return true;
}

bool D3D12CallBackManager::ID3D12GraphicsCommandList_onSetGraphicsRoot32BitConstants(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex, UINT Num32BitValuesToSet,
                                                                                     const void* pSrcData, UINT DestOffsetIn32BitValues)
{
    if (do_capture) {
        //		float* ptr;
        //		ptr = (float*)pSrcData;
        //		// to stirng array from ptr size of size/4
        //		std::string str = "[";
        //		for (int i = 0; i < Num32BitValuesToSet; i++) {
        //			str += std::to_string(ptr[i]) + ", ";
        //		}
        //		str += "]";
        // spdlog::info("SetGraphicsRoot32BitConstants: index {} with {} values {}", RootParameterIndex, Num32BitValuesToSet, str);
    }
    return true;
}

bool D3D12CallBackManager::ID3D12GraphicsCommandList_onSetGraphicsRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature)
{
    if (do_capture) {
        //		spdlog::info("ID3D12GraphicsCommandList[{:p}]->SetGraphicsRootSignature[{:p}]", fmt::ptr(pCommandList), fmt::ptr(pRootSignature));
    }
    return true;
}

bool D3D12CallBackManager::ID3D12GraphicsCommandList_onSetComputeRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature)
{
    if (do_capture) {
        //		spdlog::info("ID3D12GraphicsCommandList[{:p}]->SetComputeRootSignature[{:p}]", fmt::ptr(pCommandList), fmt::ptr(pRootSignature));
    }
    return true;
}

bool D3D12CallBackManager::ID3D12GraphicsCommandList_onSetPipelineState(ID3D12GraphicsCommandList* pCommandList, ID3D12PipelineState* pPipelineState)
{
    if (do_capture) {
        //		PSOPrivateData privateData;
        //		UINT dataSize = sizeof(privateData);
        //		if (SUCCEEDED(pPipelineState->GetPrivateData(__uuidof(PSOPrivateData), &dataSize, &privateData)))
        //		{
        //			spdlog::info("ID3D12GraphicsCommandList[{:p}]->SetPipelineStat[{:p}]e: vs_crc: 0x{:x} ps_crc: 0x{:x} cs_crc: 0x{:x}", fmt::ptr(pCommandList),
        //fmt::ptr(pPipelineState), privateData.vs_crc, privateData.ps_crc, privateData.cs_crc);
        //		}
    }
    return true;
}

bool D3D12CallBackManager::ID3D12Device_CreateCommittedResource(ID3D12Device* pDevice, const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags,
                                                                const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState,
                                                                const D3D12_CLEAR_VALUE* pOptimizedClearValue, REFIID riidResource, void** ppvResource)
{
    // spdlog::info("CreateCommittedResource: {} bytes", pDesc->Width);
    return true;
}

bool D3D12CallBackManager::ID3D12Device_onCreatePlacedResource(ID3D12Device* pDevice, ID3D12Heap* pHeap, UINT64 HeapOffset, const D3D12_RESOURCE_DESC* pDesc,
                                                               D3D12_RESOURCE_STATES InitialState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, REFIID riid, void** ppvResource)
{
    // spdlog::info("CreatePlacedResource: {} bytes", pDesc->Width);
    return true;
}

void D3D12CallBackManager::IDXGIFactory2_onBeforeCreateSwapChain(IDXGIFactory2* pFactory2, IUnknown* pUnknown, DXGI_SWAP_CHAIN_DESC1* pDesc1, IDXGISwapChain1** pChain1)
{
    spdlog::info("IDXGIFactory2_onBeforeCreateSwapChain {} {}", pDesc1->Width, pDesc1->Height);
}

void D3D12CallBackManager::IDXGIFactory2_onBeforeCreateSwapChainForHwnd(IDXGIFactory2* pFactory2, IUnknown* pUnknown, HWND pHwnd, DXGI_SWAP_CHAIN_DESC* pDesc,
                                                                        const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc1, IDXGIOutput* pOutput, IDXGISwapChain1** pChain1)
{
    // pDesc->BufferDesc.Width = 2*pDesc->BufferDesc.Width;
    spdlog::info("IDXGIFactory2_onBeforeCreateSwapChainForHwnd {} {}", pDesc->BufferDesc.Width, pDesc->BufferDesc.Height);
}

void D3D12CallBackManager::ID3D12GraphicsCommandList_onRSSetViewports(ID3D12GraphicsCommandList* pList, UINT i, D3D12_VIEWPORT* pViewport) {
/*    if(pViewport->Width > 1000 && pViewport->TopLeftY > 30) {
        dummy_counter++;
        if (dummy_counter % 3000 == 0) {
            spdlog::info("breakpoint here");
        }
        pViewport->TopLeftX = pViewport->TopLeftX + 100;
        pViewport->TopLeftY = pViewport->TopLeftY + 100;
        pViewport->Width = pViewport->Width - 200;
        pViewport->Height = pViewport->Height - 200;
    }
    spdlog::info("RSSetViewports: {} {} {} {} {} {}", pViewport->TopLeftX, pViewport->TopLeftY, pViewport->Width, pViewport->Height , pViewport->MinDepth, pViewport->MaxDepth);*/
}

void D3D12CallBackManager::ID3D12GraphicsCommandList_onSetScissorRects(ID3D12GraphicsCommandList* pList, UINT i, const D3D12_RECT* pRect) {
//    if(pRect->right > 1000) {
//        pRect->left = pRect->left + 100;
//        pRect->right = pRect->right + 100;
//        pRect->Width = pRect->Width - 200;
//        pRect->Height = pRect->Height - 200;
//    }
//    spdlog::info("SetScissorRects: {} {} {} {}", pRect->left, pRect->top, pRect->right, pRect->bottom);
}
