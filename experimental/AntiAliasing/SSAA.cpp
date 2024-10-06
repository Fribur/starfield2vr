#include "SSAA.h"
#include <RenderTargetState.h>
#include <SFVR.hpp>
#include <SpriteBatch.h>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>

namespace SHADERS::SSAA
{
    namespace PS {
#include "shader-headers/ssaa_ps.h"
    }

    namespace VS {
#include "shader-headers/rectangle_vs.h"
    }
}

using namespace DirectX::DX12;
using namespace DirectX;

void d3d12::SSAA::Apply(ID3D12Resource* backbuffer)
{
    auto command_list = m_backbuffer_copy.commands.cmd_list.Get();
    m_backbuffer_copy.commands.wait(INFINITE);

    m_backbuffer_copy.commands.copy(backbuffer, m_backbuffer_copy.texture.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PRESENT);
    float clear_color[4]{0.0f, 0.0f, 0.0f, 0.0f};

    const auto render_target_desc = m_render_target.texture->GetDesc();

    D3D12_VIEWPORT viewport{};
    viewport.Width = (float)render_target_desc.Width;
    viewport.Height = (float)render_target_desc.Height;
    viewport.MinDepth = D3D12_MIN_DEPTH;
    viewport.MaxDepth = D3D12_MAX_DEPTH;

    D3D12_RECT scissor_rect{};
    scissor_rect.left = 0;
    scissor_rect.top = 0;
    scissor_rect.right = (LONG)render_target_desc.Width;
    scissor_rect.bottom = (LONG)render_target_desc.Height;

    // second pass

    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_render_target.texture.Get();

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    command_list->ResourceBarrier(1, &barrier);

    m_backbuffer_copy.commands.clear_rtv(m_render_target, clear_color, D3D12_RESOURCE_STATE_PRESENT);


    D3D12_CPU_DESCRIPTOR_HANDLE rtv_heaps[] = { m_render_target.get_rtv() };
    command_list->OMSetRenderTargets(1, rtv_heaps, FALSE, nullptr);
    command_list->RSSetViewports(1, &viewport);
    command_list->RSSetScissorRects(1, &scissor_rect);

    ID3D12DescriptorHeap* game_heaps[] = { m_backbuffer_copy.srv_heap->Heap() };
    command_list->SetDescriptorHeaps(1, game_heaps);

    command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    command_list->SetGraphicsRootSignature( m_rootSignature.Get());
    command_list->SetPipelineState(m_pipelineState.Get());

    command_list->SetGraphicsRootDescriptorTable(0, m_backbuffer_copy.get_srv_gpu());

    command_list->DrawInstanced(3, 1, 0, 0);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    command_list->ResourceBarrier(1, &barrier);
    m_backbuffer_copy.commands.execute();
}

bool d3d12::SSAA::setup(ID3D12Device* device, const D3D12_RESOURCE_DESC backBuffer_desc)
{
    auto backbuffer_srv_desc = backBuffer_desc;
    backbuffer_srv_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    backbuffer_srv_desc.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    auto rt_desc = backBuffer_desc;

    rt_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rt_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    rt_desc.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    D3D12_HEAP_PROPERTIES heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    {
        ComPtr<ID3D12Resource> backbuffer_copy{};
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &backbuffer_srv_desc, D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr,
                                                   IID_PPV_ARGS(backbuffer_copy.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create backbuffer copy for SSAA.");
            return false;
        }
        if(!m_backbuffer_copy.setup(device, backbuffer_copy.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, L"SSAA Backbuffer Copy")) {
            spdlog::error("[VR] Error setting up backbuffer copy for SSAA. SRV/RTV");
            return false;
        }
    }

    {
        ComPtr<ID3D12Resource> fxaa_output{};
        rt_desc.Width = backBuffer_desc.Width/3;
        rt_desc.Height = backBuffer_desc.Height/3;
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &rt_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(fxaa_output.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create SSAA output texture.");
            return false;
        }

        if (!m_render_target.setup(device, fxaa_output.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM)) {
            spdlog::error("[VR] Error setting up SSAA texture RTV/SRV.");
            return false;
        }
    }


    if (!CreateSSAARootSignature(device) || !CreateSSAAPipelineState(device, DXGI_FORMAT_R8G8B8A8_UNORM))
    {
        spdlog::error("Failed to setup SSAA");
        return false;
    }

    return true;
}

bool d3d12::SSAA::CreateSSAARootSignature(ID3D12Device* device)
{
    HRESULT                 hr;
    CD3DX12_DESCRIPTOR_RANGE1 fxaaDescriptorRange[1];
    CD3DX12_ROOT_PARAMETER1 fxaaRootParams[1];

    fxaaDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);
    fxaaRootParams[0].InitAsDescriptorTable(1, &fxaaDescriptorRange[0], D3D12_SHADER_VISIBILITY_PIXEL);

    CD3DX12_STATIC_SAMPLER_DESC staticSamplers[1];//(0, D3D12_FILTER_ANISOTROPIC);
    staticSamplers[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
                                                    | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(fxaaRootParams), fxaaRootParams,
                               _countof(staticSamplers), staticSamplers, rootSignatureFlags);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, signature.GetAddressOf(), error.GetAddressOf());
    if (FAILED(hr)) {
        if (error)
            spdlog::error("Error: D3DX12SerializeVersionedRootSignature {}", static_cast<char*>(error->GetBufferPointer()));
        return false;
    }
    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.GetAddressOf()));
    if (FAILED(hr)) {
        if (error)
            spdlog::error("Error: CreateRootSignature {}", static_cast<char*>(error->GetBufferPointer()));
        return false;
    }
    spdlog::info("[VR] SSAA root signature created.");
    return true;
}

bool d3d12::SSAA::CreateSSAAPipelineState(ID3D12Device* device, DXGI_FORMAT backBufferFormat)
{
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC ssaaPSODesc = {};
        ssaaPSODesc.InputLayout = { };
        ssaaPSODesc.pRootSignature = m_rootSignature.Get();
        ssaaPSODesc.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SSAA::VS::g_vs_main, sizeof(SHADERS::SSAA::VS::g_vs_main));
        ssaaPSODesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SSAA::PS::g_ps_main, sizeof(SHADERS::SSAA::PS::g_ps_main));
        ssaaPSODesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        ssaaPSODesc.DepthStencilState.DepthEnable = false;
        ssaaPSODesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        ssaaPSODesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
        ssaaPSODesc.SampleMask = UINT_MAX;
        ssaaPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        ssaaPSODesc.NumRenderTargets = 1;
        ssaaPSODesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
        ssaaPSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R10G10B10A2_UNORM
        ssaaPSODesc.SampleDesc.Count = 1;
        ThrowIfFailed(device->CreateGraphicsPipelineState(&ssaaPSODesc, IID_PPV_ARGS(m_pipelineState.GetAddressOf())));
        spdlog::info("[VR] SSAA pipeline state created.");
    }

    return true;
}

