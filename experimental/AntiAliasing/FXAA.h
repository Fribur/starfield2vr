#pragma once
#include <d3d12.h>
//#include <wrl/client.h>
#include "TextureContext.hpp"
#include <_deps/directxtk12-src/Inc/SpriteBatch.h>
#include <_deps/directxtk12-src/Inc/CommonStates.h>
#include <_deps/directxtk12-src/Inc/DescriptorHeap.h>
#include <_deps/directxtk12-src/Inc/ResourceUploadBatch.h>
#include <_deps/directxtk12-src/Inc/ScreenGrab.h>
#include <_deps/directxtk12-src/Inc/SimpleMath.h>
#include <_deps/directxtk12-src/Inc/VertexTypes.h>
#include <d3dcompiler.h>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace d3d12
{
    struct FXAA
    {
        bool setup(ID3D12Device* device, D3D12_RESOURCE_DESC backBuffer_desc);
        void reset() {
            p_fxaa_pso.Reset();
            m_luma_pso.Reset();
            m_rootSignature.Reset();
            m_resourceDescriptors.reset();
            m_render_target.reset();
            m_backbuffer_copy.reset();
            m_sprite_batch.reset();
        }

        void Apply(ID3D12Resource* source);

        bool CreateFXAARootSignature(ID3D12Device* device);
        bool CreateFXAAPipelineState(ID3D12Device* device, DXGI_FORMAT backBufferFormat);

        ComPtr<ID3D12RootSignature> m_rootSignature;
        ComPtr<ID3D12PipelineState>  p_fxaa_pso;
        ComPtr<ID3D12PipelineState>  m_luma_pso;
        ComPtr<ID3D12PipelineState>  m_downsample_pso;

        std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
        d3d12::TextureContext           m_backbuffer_copy{};
        d3d12::TextureContext           m_backbuffer_luma{};
        d3d12::TextureContext           m_fxaa_out{};
        d3d12::TextureContext           m_render_target{};
        void                            setup_sprite_batch_pso(DXGI_FORMAT output_format);
        std::unique_ptr<DirectX::DX12::SpriteBatch>            m_sprite_batch;
        void convert_texture(ID3D12GraphicsCommandList* command_list, const TextureContext& src, const TextureContext& dst, D3D12_RESOURCE_STATES src_state,
                             D3D12_RESOURCE_STATES dst_state);
    };
}