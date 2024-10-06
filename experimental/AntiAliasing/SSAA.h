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
    struct SSAA
    {
        bool setup(ID3D12Device* device, D3D12_RESOURCE_DESC backBuffer_desc);
        void reset() {
            m_pipelineState.Reset();
            m_rootSignature.Reset();
            m_resourceDescriptors.reset();
            m_render_target.reset();
            m_backbuffer_copy.reset();
        }

        void Apply(ID3D12Resource* source);

        bool CreateSSAARootSignature(ID3D12Device* device);
        bool CreateSSAAPipelineState(ID3D12Device* device, DXGI_FORMAT backBufferFormat);

        ComPtr<ID3D12RootSignature> m_rootSignature;
        ComPtr<ID3D12PipelineState> m_pipelineState;

        std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
        d3d12::TextureContext           m_backbuffer_copy{};
        d3d12::TextureContext           m_render_target{};
    };
}