#pragma once

#include "TAADebugView.h"
#include <CreationEngine/memory/offsets.h>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <mods/VR.hpp>
#include "CreationEngineRendererModule.h"
#include "CreationEngineConstants.h"
namespace SHADERS::TAA::DEBUG
{
    namespace VS
    {
#include "shader-headers/taa_debug_vs.h"
    }

    namespace PS
    {
#include "CreationEngineConstants.h"
#include "shader-headers/taa_debug_ps.h"
    }
} // namespace SHADERS::TAA::DEBUG

TaaSettings  g_taa_settings{};

void TAADebugView::InstallHooks()
{
//    REL::Relocation<uintptr_t> taa_vfunc7_hook_addr{ REL::ID(208939) };
//    taa_vfunc7_hook = std::make_unique<PolyHook2FunctionHook>(taa_vfunc7_hook_addr.address(), reinterpret_cast<uint64_t>(&TAADebugView::onTaaPass));
//    taa_vfunc7_hook->create();
}

D3D12_SHADER_RESOURCE_VIEW_DESC get_srv_desc(DXGI_FORMAT source) {
    D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    desc.Texture2D.MipLevels = 1;
    desc.Texture2D.MostDetailedMip = 0;
    desc.Texture2D.PlaneSlice = 0;
    desc.Texture2D.ResourceMinLODClamp = 0.0f;
    switch (source) {
    case DXGI_FORMAT_R8G8_TYPELESS:
        desc.Format = DXGI_FORMAT_R8G8_UNORM;
        break;
    case DXGI_FORMAT_R11G11B10_FLOAT:
        desc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
        break;
    case DXGI_FORMAT_R16G16_TYPELESS:
        desc.Format = DXGI_FORMAT_R16G16_FLOAT;
        break;
    case DXGI_FORMAT_R8_TYPELESS:
        desc.Format = DXGI_FORMAT_R8_UNORM;
        break;
    case DXGI_FORMAT_R16_TYPELESS:
        desc.Format = DXGI_FORMAT_R16_UNORM;
        break;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        desc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        break;
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        break;
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    default:
        desc.Format = source;  // Use the source format if not specified
        break;
    }
    return desc;

}

void TAADebugView::InitResources(const D3D12_RESOURCE_DESC& desc, ComPtr<ID3D12Resource>& destination, SRV srv) {
    if (inited.load()) {
        auto device = g_framework->get_d3d12_hook()->get_device();

        auto current_desc  = destination == nullptr ? D3D12_RESOURCE_DESC{} : destination->GetDesc();
        if (destination == nullptr || current_desc.Width != desc.Width || current_desc.Height != desc.Height || current_desc.Format != desc.Format)
        {
            std::scoped_lock _{mtx};
            {
                spdlog::info("[VR] Creating resource copy for TAA. w={} h={} format={}", desc.Width, desc.Height, (UINT)desc.Format);
                destination.Reset();

                D3D12_HEAP_PROPERTIES heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

                if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D) {
                    spdlog::error("[VR] TAA resource copy only supports 2D textures.");
                    return;
                }
                auto desc_copy = desc;
                desc_copy.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
                desc_copy.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
                auto initial_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                D3D12_CLEAR_VALUE clearValue = {};
                if(desc_copy.Format == DXGI_FORMAT_R32G8X24_TYPELESS || desc_copy.Format == DXGI_FORMAT_R16_TYPELESS || desc_copy.Format == DXGI_FORMAT_R8_TYPELESS) {
//                    desc_copy.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
                    initial_state |= D3D12_RESOURCE_STATE_DEPTH_READ;
                    desc_copy.Flags &= ~D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
                    desc_copy.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
                    switch (desc_copy.Format) {
                    case DXGI_FORMAT_R32G8X24_TYPELESS:
                        clearValue.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
                        break;
                    case DXGI_FORMAT_R16_TYPELESS:
                        clearValue.Format = DXGI_FORMAT_D16_UNORM;
                        break;
                    case DXGI_FORMAT_R8_TYPELESS:
                        clearValue.Format = DXGI_FORMAT_R8_UNORM;
                        break;
                    default:
                        break;
                    }
                    clearValue.DepthStencil.Depth = 1.0f;
                    clearValue.DepthStencil.Stencil = 0;
                }
                if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &desc_copy, initial_state, nullptr,
                                                           IID_PPV_ARGS(destination.GetAddressOf()))))
                {
                    spdlog::error("[VR] Failed to create resource copy copy for TAA.");
                    return;
                }

                auto srv_desc = get_srv_desc(desc_copy.Format);
                device->CreateShaderResourceView(destination.Get(), &srv_desc, m_srv_heap->GetCpuHandle((UINT)srv));
            }
        }
    }
}


void TAADebugView::Init(ID3D12Device* device)
{
        spdlog::info("Setting up TAA debug layer D3D12");

        try {
            m_srv_heap = std::make_unique<DirectX::DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, (UINT)SRV::COUNT);
        }
        catch (...) {
            spdlog::error("Failed to create SRV/RTV descriptor heap for TAA");
            return;
        }
        spdlog::info("[VR] TAA debug layer D3D12 resrouce setup complete.");

        if (!CreateRootSignature(device) || !CreatePipelineStates(device)) {
            spdlog::error("[VR] Failed to create TAA root signature.");
            return;
        }
        inited.exchange(true);
}

bool TAADebugView::CreateRootSignature(ID3D12Device* device)
{
    HRESULT                   hr;
    CD3DX12_DESCRIPTOR_RANGE1 descriptorRange[1];
    CD3DX12_ROOT_PARAMETER1   rootParams[2];

    descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, (UINT)SRV::COUNT, 0, 0);
    rootParams[0].InitAsDescriptorTable(1, &descriptorRange[0], D3D12_SHADER_VISIBILITY_PIXEL);
    ;
    rootParams[1].InitAsConstants(4, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
    CD3DX12_STATIC_SAMPLER_DESC staticSamplers[2];

    // D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT
    staticSamplers[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // D3D12_FILTER_MIN_MAG_MIP_POINT
    staticSamplers[1].Init(1, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[1].ShaderVisibility            = D3D12_SHADER_VISIBILITY_PIXEL;
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
                                                    | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParams), rootParams, _countof(staticSamplers), staticSamplers, rootSignatureFlags);

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
    spdlog::info("[VR] TAA root signature created.");
    return true;
}

bool TAADebugView::CreatePipelineStates(ID3D12Device* device)
{
    // debug mvec pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC debugTaa_pso = {};
        debugTaa_pso.InputLayout                        = {};
        debugTaa_pso.pRootSignature                     = m_rootSignature.Get();
        debugTaa_pso.VS                                 = CD3DX12_SHADER_BYTECODE(&SHADERS::TAA::DEBUG::VS::g_vs_main, sizeof(SHADERS::TAA::DEBUG::VS::g_vs_main));
        debugTaa_pso.PS                                 = CD3DX12_SHADER_BYTECODE(&SHADERS::TAA::DEBUG::PS::g_ps_main, sizeof(SHADERS::TAA::DEBUG::PS::g_ps_main));

        auto blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//        blendDesc.AlphaToCoverageEnable = FALSE;
//        blendDesc.IndependentBlendEnable = FALSE;
//        blendDesc.RenderTarget[0].BlendEnable = TRUE;
//        blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
//        blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
//        blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
//        blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
//        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//        blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//        blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        debugTaa_pso.BlendState = blendDesc;
        debugTaa_pso.DepthStencilState             = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        debugTaa_pso.DepthStencilState.DepthEnable = false;
        debugTaa_pso.SampleMask                    = UINT_MAX;
        debugTaa_pso.RasterizerState               = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        debugTaa_pso.PrimitiveTopologyType         = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        debugTaa_pso.NumRenderTargets              = 1;
        debugTaa_pso.DSVFormat                     = DXGI_FORMAT_UNKNOWN;
        debugTaa_pso.RTVFormats[0]                 = DXGI_FORMAT_R8G8B8A8_UNORM;
        debugTaa_pso.SampleDesc.Count              = 1;
        debugTaa_pso.SampleDesc.Quality            = 0;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&debugTaa_pso, IID_PPV_ARGS(visualize_resource_pso.GetAddressOf())));
        spdlog::info("[VR] debug TAA resource pso.");
    }
    return true;
}

void TAADebugView::Apply(ID3D12GraphicsCommandList* commandList, ID3D12Resource* backbuffer, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle)
{
    if (g_taa_settings.enabled == 0 || !inited.load() || resource_copy == nullptr || resource_comp == nullptr) {
        return;
    }

    D3D12_RESOURCE_BARRIER barriers[1]{
        CD3DX12_RESOURCE_BARRIER::Transition(backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET),
    };

    const auto render_target_desc = backbuffer->GetDesc();
    float      clear_color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

    D3D12_VIEWPORT viewport{};
//    viewport.TopLeftX = 38.0f;
    viewport.Width    = (float)render_target_desc.Width/2;
    viewport.Height   = (float)render_target_desc.Height;
    viewport.MinDepth = D3D12_MIN_DEPTH;
    viewport.MaxDepth = D3D12_MAX_DEPTH;

    D3D12_RECT scissor_rect{};
    scissor_rect.left   = 0;
    scissor_rect.top    = 0;
    scissor_rect.right  = (LONG)render_target_desc.Width/2;
    scissor_rect.bottom = (LONG)render_target_desc.Height;

    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissor_rect);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    float gamma = g_taa_settings.gamma;
    float contrast = g_taa_settings.contrast;
    float temperature = g_taa_settings.temperature;
    float exposure = g_taa_settings.exposure;
    auto desc = resource_copy->GetDesc();
    int format1 = (int)desc.Format;
    int width = desc.Width;
    int height = desc.Height;
    auto desc2 = resource_comp->GetDesc();
    int format2 = (int)desc2.Format;
    commandList->SetGraphicsRoot32BitConstants(1, 1, &width, 0);
    commandList->SetGraphicsRoot32BitConstants(1, 1, &height, 1);
    commandList->SetGraphicsRoot32BitConstants(1, 1, &format1, 2);
    commandList->SetGraphicsRoot32BitConstants(1, 1, &format2, 3);
    ID3D12DescriptorHeap* descriptorHeaps[] = { m_srv_heap->Heap() };
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    commandList->SetGraphicsRootDescriptorTable(0, m_srv_heap->GetFirstGpuHandle());

    {
        commandList->SetPipelineState(visualize_resource_pso.Get());
//        commandList->ClearRenderTargetView(*rtv_handle, clear_color, 1, &scissor_rect);
        commandList->OMSetRenderTargets(1, rtv_handle, FALSE, nullptr);
        commandList->DrawInstanced(6, 1, 0, 0);
    }

    barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
    commandList->ResourceBarrier(1, barriers);
}

uintptr_t TAADebugView::onTaaPass(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, RE::CreationRendererPrivate::RenderPassData* renderPassData)
{
    auto instance    = Get();
    auto original_fn = instance->taa_vfunc7_hook->get_original<decltype(TAADebugView::onTaaPass)>();
    if(g_taa_settings.enabled && instance->inited.load()) {
        auto context = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(pRenderGraphData->getCommandList());
        auto commandList = context->pID3D12CommandList;
        auto fc = CreationEngineRendererModule::GetGlobalFrameCount();
        if(g_taa_settings.debugOutput > -1)
        {
            auto resource = pRenderGraphData->getResourceByIndex(g_taa_settings.debugOutput, fc);
            if(resource != nullptr) {
                instance->InitResources(resource->GetDesc(), instance->resource_copy, SRV::DEBUG_RESOURCE);
                CreationEngineRendererModule::CopyResource(commandList,resource, instance->resource_copy.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            }

        }
        if(g_taa_settings.debugOutput2 > -1)
        {
            auto resource = pRenderGraphData->getResourceByIndex(g_taa_settings.debugOutput2, fc-1);
            if(resource != nullptr)
            {
                instance->InitResources(resource->GetDesc(), instance->resource_comp, SRV::DEBUG_RESOURCE_COMP);
                CreationEngineRendererModule::CopyResource(commandList, resource, instance->resource_comp.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                           D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            }
        }
    }
    auto result = original_fn(pPass, pRenderGraphData, renderPassData);
    auto fc       = CreationEngineRendererModule::GetGlobalFrameCount();
    auto context     = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(pRenderGraphData->getCommandList());
    auto commandList = context->pID3D12CommandList;
    auto vr = VR::get();

    if(vr->is_hmd_active() && Constants::enabledResourcesToCopy[2]) {
        auto resource = pRenderGraphData->getResourceByIndex(2, (fc - 1) & 1);
        CreationEngineRendererModule::Get()->SwapBuffer(commandList, resource, 2,  (fc - 1) & 1, fc & 1);
    }
    if(vr->is_hmd_active() && Constants::enabledResourcesToCopy[3]) {
        auto resource = pRenderGraphData->getResourceByIndex(3, (fc - 1) & 1);
        CreationEngineRendererModule::Get()->SwapBuffer(commandList, resource, 3,  (fc - 1) & 1, fc & 1);
    }
    return result;
}
