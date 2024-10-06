#pragma once
#include "vr/d3d12/TextureContext.hpp"
#include <_deps/directxtk12-src/Inc/CommonStates.h>
#include <_deps/directxtk12-src/Inc/DescriptorHeap.h>
#include <_deps/directxtk12-src/Inc/ResourceUploadBatch.h>
#include <_deps/directxtk12-src/Inc/ScreenGrab.h>
#include <_deps/directxtk12-src/Inc/SimpleMath.h>
#include <_deps/directxtk12-src/Inc/SpriteBatch.h>
#include <_deps/directxtk12-src/Inc/VertexTypes.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <utility/Config.hpp>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::SimpleMath;


struct SMAA
{
public:

    virtual ~SMAA() {
        reset();
    };

    bool setup(ID3D12Device* device, D3D12_RESOURCE_DESC backBuffer_desc);
    inline void reset() {
        spdlog::info("Resetting SMAA");
        m_depthLinearPso.Reset();
        m_edgeDetectionLumaPso.Reset();
        m_edgeDetectionLumaPredicationPso.Reset();
        m_edgeDetectionColorPso.Reset();
        m_edgeDetectionColorPredicationPso.Reset();
        m_edgeDetectionDepthPso.Reset();
        m_blendingWeightCalcPso.Reset();
        m_finalBlendPso.Reset();
        m_dummy_pso.Reset();
        m_rootSignature.Reset();

        m_rtv_heap.reset();
        m_dsv_heap.reset();
        m_srv_heap.reset();

        m_backbuffer_copy_resource.Reset();
        m_SMAAEdge_resource.Reset();
        m_SMAABlend_resource.Reset();
        m_areaTex_resource.Reset();
        m_searchTex_resource.Reset();
        m_depthStencilBuffer_srv_resource.Reset();
        m_depthStencilBuffer_linear_resource.Reset();
        m_depthStencilBuffer.Reset();
    }

    void Apply(std::unique_ptr<d3d12::CommandContext>& cmd_ctx, ID3D12Resource* pResource, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle,  ID3D12Resource* dsv);
private:
    void create_precompiled_textures(ID3D12Device* device);
    bool CreateRootSignature(ID3D12Device* device);
    bool CreatePipelineStates(ID3D12Device* device, DXGI_FORMAT backBufferFormat);
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> m_depthLinearPso;
    ComPtr<ID3D12PipelineState> m_edgeDetectionLumaPso;
    ComPtr<ID3D12PipelineState> m_edgeDetectionLumaPredicationPso;
    ComPtr<ID3D12PipelineState> m_edgeDetectionColorPso;
    ComPtr<ID3D12PipelineState> m_edgeDetectionColorPredicationPso;
    ComPtr<ID3D12PipelineState> m_edgeDetectionDepthPso;
    ComPtr<ID3D12PipelineState> m_blendingWeightCalcPso;
    ComPtr<ID3D12PipelineState> m_finalBlendPso;
    ComPtr<ID3D12PipelineState> m_dummy_pso;

    // Resources
    ComPtr<ID3D12Resource> m_backbuffer_copy_resource;
    ComPtr<ID3D12Resource> m_SMAAEdge_resource;
    ComPtr<ID3D12Resource> m_SMAABlend_resource;
    ComPtr<ID3D12Resource> m_areaTex_resource;
    ComPtr<ID3D12Resource> m_searchTex_resource;
    ComPtr<ID3D12Resource> m_depthStencilBuffer_srv_resource;
    ComPtr<ID3D12Resource> m_depthStencilBuffer_linear_resource;
    ComPtr<ID3D12Resource> m_depthStencilBuffer;

    std::unique_ptr<DirectX::DescriptorHeap>  m_dsv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_srv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_rtv_heap{};

    enum class SRV : int {
        BACKBUFFER_COPY,
        BACKBUFFER_COPY_SRGB,
        SMAA_EDGE,
        SMAA_BLEND_WEIGHT,
        AREA_TEX,
        SEARCH_TEX,
        DEPTH_BUFFER_READ,
        DEPTH_BUFFER_LINEAR,
        DEBUG_SRV,
        COUNT
    };

    enum class RTV : int {
        SMAA_EDGE,
        SMAA_BLEND_WEIGHT,
        DEPTH_BUFFER_LINEAR,
        MAIN_RT_SRGB,
        COUNT
    };

};
