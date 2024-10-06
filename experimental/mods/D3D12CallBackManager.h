#pragma once
#include "Xinput.h"
#include "framework.h"
#include <Dxgi1_3.h>
#include <d3d12.h>
#include <dxgi.h>
#include <unordered_set>
#include <windows.h>
#include <RE/N/NiCamera.h>
#include <RE/N/NiMatrix3.h>

struct __declspec(uuid("67227C46-7E94-4D3E-B97D-CDB5FFC51B53")) PSOPrivateData
{
    uint32_t ps_crc;
    uint32_t vs_crc;
    uint32_t cs_crc;
};

class D3D12CallBackManager
{
public:
    D3D12CallBackManager(HINSTANCE hModule);
    ~D3D12CallBackManager();
    D3D12CallBackManager(const D3D12CallBackManager&)            = delete;
    D3D12CallBackManager& operator=(const D3D12CallBackManager&) = delete;

    virtual bool XInput1_4_onGetState(UINT dwUserIndex, XINPUT_STATE* pState);
    virtual bool IDXGISwapChain_onBeforePresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    virtual bool IDXGISwapChain_onAfterPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    virtual bool ID3D12Resource_onMap(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pReadRange, void** ppData);
    virtual bool ID3D12Resource_onUnmap(ID3D12Resource* pResource, UINT Subresource, const D3D12_RANGE* pWrittenRange);
    virtual bool onMemcpy(void* dst, const void* src, size_t size);
    virtual bool ID3D12GraphicsCommandList_onSetGraphicsRootConstantBufferView(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex,
                                                                               D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);
    virtual bool ID3D12Device_CreateConstantBufferView(ID3D12Device* pDevice, const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
    virtual bool ID3D12Device_CreateRootSignature(ID3D12Device* pDevice, UINT nodeMask, const void* pBlobWithRootSignature, SIZE_T blobLengthInBytes, REFIID riid,
                                                  void** ppvRootSignature);
    virtual bool ID3D12Device_CreateGraphicsPipelineState(ID3D12Device* pDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid, void** ppPipelineState);
    virtual bool ID3D12Device_CreatePipelineLibrary(ID3D12Device1* pDevice, const void* pLibraryBlob, SIZE_T BlobLength, REFIID riid, void** ppPipelineLibrary);
    void         parse_pipline_stream(const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, PSOPrivateData* privateData);
    virtual bool ID3D12Device_CreatePipelineState(ID3D12Device* pDevice, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid, void** ppPipelineState);
    virtual bool ID3D12GraphicsCommandList_onSetGraphicsRoot32BitConstants(ID3D12GraphicsCommandList* pCommandList, UINT RootParameterIndex, UINT Num32BitValuesToSet,
                                                                           const void* pSrcData, UINT DestOffsetIn32BitValues);
    virtual bool ID3D12GraphicsCommandList_onSetGraphicsRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature);
    virtual bool ID3D12GraphicsCommandList_onSetComputeRootSignature(ID3D12GraphicsCommandList* pCommandList, ID3D12RootSignature* pRootSignature);
    virtual bool ID3D12GraphicsCommandList_onSetPipelineState(ID3D12GraphicsCommandList* pCommandList, ID3D12PipelineState* pPipelineState);
    virtual bool ID3D12Device_CreateCommittedResource(ID3D12Device* pDevice, const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags,
                                                      const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                                                      REFIID riidResource, void** ppvResource);
    virtual bool ID3D12Device_onCreatePlacedResource(ID3D12Device* pDevice, ID3D12Heap* pHeap, UINT64 HeapOffset, const D3D12_RESOURCE_DESC* pDesc,
                                                     D3D12_RESOURCE_STATES InitialState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, REFIID riid, void** ppvResource);
    virtual bool onID3D12PipelineLibrary_LoadGraphicsPipeline(ID3D12PipelineLibrary* pPipelineLibrary, LPCWSTR pName, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, REFIID riid,
                                                              void** ppPipelineState);
    virtual bool onID3D12PipelineLibrary_LoadPipeline(ID3D12PipelineLibrary1* pPipelineLibrary, LPCWSTR pName, const D3D12_PIPELINE_STATE_STREAM_DESC* pDesc, REFIID riid,
                                                      void** ppPipelineState);
    void IDXGIFactory2_onBeforeCreateSwapChain(IDXGIFactory2* pFactory2, IUnknown* pUnknown, DXGI_SWAP_CHAIN_DESC1* pDesc1, IDXGISwapChain1** pChain1);

    void IDXGIFactory2_onBeforeCreateSwapChainForHwnd(IDXGIFactory2* pFactory2, IUnknown* pUnknown, HWND pHwnd, DXGI_SWAP_CHAIN_DESC* pDesc,
                                                      const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc1, IDXGIOutput* pOutput, IDXGISwapChain1** pChain1);

    void ID3D12GraphicsCommandList_onRSSetViewports(ID3D12GraphicsCommandList* pList, UINT i, D3D12_VIEWPORT* pViewport);

    void ID3D12GraphicsCommandList_onSetScissorRects(ID3D12GraphicsCommandList* pList, UINT i, const D3D12_RECT* pRect);

private:
    HINSTANCE                    hModule;
    std::unordered_set<uint64_t> cpuHandlesMap;
    bool                         do_capture    = false;
    float                        separation    = 1.0f;
    unsigned long                frameCount    = 0;
    unsigned long                dummy_counter = 0;
};

extern D3D12CallBackManager* g_d3d12_callback_manager;
