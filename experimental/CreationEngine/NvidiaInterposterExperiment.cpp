//
// Created by sergp on 8/18/2024.
//

#include "NvidiaInterposterExperiment.h"
#include "sl_matrix_helpers.h"
#include <CreationEngine/RE2/offsets.h>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <mods/VR.hpp>

HMODULE g_interposer = nullptr;

namespace SHADERS::DLSS::DEBUG
{
    namespace VS
    {
#include "shader-headers/dlss_debug_vs.h"
    }

    namespace PS
    {
#include "shader-headers/dlss_debug_ps.h"
    }
} // namespace SHADERS::DLSS::DEBUG

void NvidiaInterposterExperiment::InstallHooks()
{
    if (g_interposer) {
        return;
    }
    spdlog::info("Installing sl.interposer hooks");
    while ((g_interposer = GetModuleHandle("sl.interposer.dll")) == nullptr) {
        std::this_thread::yield();
    }

//    auto getNewFrameTokenFn = GetProcAddress(g_interposer, "slGetNewFrameToken");
//    m_get_new_frame_token_hook = std::make_unique<FunctionHook>((void **)getNewFrameTokenFn, (void *) &DlssDualView::on_slGetNewFrameToken);
//    if(!m_get_new_frame_token_hook->create()) {
//        spdlog::error("Failed to hook slGetNewFrameToken");
//    }

    auto slSetTagFn = GetProcAddress(g_interposer, "slSetTag");
    m_set_tag_hook  = std::make_unique<FunctionHook>((void**)slSetTagFn, (void*)&NvidiaInterposterExperiment::on_slSetTag);
    if (!m_set_tag_hook->create()) {
        spdlog::error("Failed to hook slSetTag");
    }

    auto slEvaluateFeatureFn = GetProcAddress(g_interposer, "slEvaluateFeature");
    m_evaluate_feature_hook  = std::make_unique<FunctionHook>((void**)slEvaluateFeatureFn, (void*)&NvidiaInterposterExperiment::on_slEvaluateFeature);
    if (!m_evaluate_feature_hook->create()) {
        spdlog::error("Failed to hook slEvaluateFeature");
    }

    auto slSetConstantsFn = GetProcAddress(g_interposer, "slSetConstants");
    m_set_constants_hook  = std::make_unique<FunctionHook>((void**)slSetConstantsFn, (void*)&NvidiaInterposterExperiment::on_slSetConstants);
    if (!m_set_constants_hook->create()) {
        spdlog::error("Failed to hook slSetConstants");
    }

//    TODO why it is commented out
//    auto slAllocateResourcesFn = GetProcAddress(g_interposer, "slAllocateResources");
//    m_allocate_resources_hook  = std::make_unique<FunctionHook>((void**)slAllocateResourcesFn, (void*)&DlssDualView::on_slAllocateResources);
//    if (!m_allocate_resources_hook->create()) {
//        spdlog::error("Failed to hook slAllocateResources");
//    }
//
//    auto slFreeResourcesFn = GetProcAddress(g_interposer, "slFreeResources");
//    m_free_resources_hook  = std::make_unique<FunctionHook>((void**)slFreeResourcesFn, (void*)&DlssDualView::on_slFreeResources);
//    if (!m_free_resources_hook->create()) {
//        spdlog::error("Failed to hook slFreeResources");
//    }

//        REL::Relocation<uintptr_t>  getDllsFrameTokenFnAddr{REL::ID(1078687)};
//        m_onGetDllsFrameToken = std::make_unique<PolyHook2FunctionHook>(getDllsFrameTokenFnAddr.address(), (uintptr_t)&DlssDualView::on_ceGetNewFrameToken);
//        m_onGetDllsFrameToken->create();

    spdlog::info("sl.interposer Hooks installed");
}

sl::Result NvidiaInterposterExperiment::on_slGetNewFrameToken(sl::FrameToken*& token, const uint32_t* frameIndex)
{
    auto instance = NvidiaInterposterExperiment::Get();

    auto original_fn = instance->m_get_new_frame_token_hook->get_original<decltype(NvidiaInterposterExperiment::on_slGetNewFrameToken)>();
    auto vr          = VR::get();
    //    spdlog::info("slGetNewFrameToken called {} frame {}", vr->get_current_render_eye() == VRRuntime::Eye::RIGHT,*frameIndex);
    //
    //    if(vr->is_hmd_active()  && !vr->m_left_trigger_down && vr->get_current_render_eye() == VRRuntime::Eye::RIGHT && instance->m_afr_frame_token != nullptr) {
    //        token = instance->m_afr_frame_token;
    //        return sl::Result::eOk;
    //
    //    }
    //    if(instance->first_frame == 0) {
    //        instance->first_frame = *frameIndex;
    //    }
    auto half_frame = *frameIndex / 2;
    auto result     = original_fn(token, frameIndex);
    //    original_fn(instance->m_afr_frame_token, &half_frame);
    return result;
}

sl::Result NvidiaInterposterExperiment::on_slSetTag(sl::ViewportHandle& viewport, const sl::ResourceTag* tags, uint32_t numTags, sl::CommandBuffer* cmdBuffer)
{
    auto            instance    = NvidiaInterposterExperiment::Get();
    auto            original_fn = instance->m_set_tag_hook->get_original<decltype(NvidiaInterposterExperiment::on_slSetTag)>();
    auto            vr          = VR::get();
    ID3D12Resource* mvec_resource{ nullptr };
    uint32_t mvec_state{UINT_MAX};
    ID3D12Resource* depth_resource{ nullptr };
    uint32_t depth_state{UINT_MAX};

    for (int i = 0; i < numTags; i++) {
        if(tags[i].resource == nullptr) {
            continue;
        }
        if (tags[i].type == sl::kBufferTypeMotionVectors) {
            mvec_resource = *tags[i].resource;
            mvec_state = tags[i].resource->state;
        }
        else if (tags[i].type == sl::kBufferTypeScalingInputColor) {
            auto         colorInExt   = tags[i].extent;
            unsigned int renderWidth  = colorInExt.width;
            unsigned int renderHeight = colorInExt.height;
            if (renderWidth == 0 || renderHeight == 0) {
                ID3D12Resource* d3d12_resource = *tags[i].resource;
                auto            desc           = d3d12_resource->GetDesc();
                renderWidth                    = desc.Width;
                renderHeight                   = desc.Height;
            }
            g_dlss_constants.texSize = { (float)renderWidth, (float)renderHeight, 1.0f / (float)renderWidth, 1.0f / (float)renderHeight };
        }
        else if (tags[i].type == sl::kBufferTypeDepth) {
            depth_resource = *tags[i].resource;
            depth_state = tags[i].resource->state;
        }
    }
    if (mvec_resource != nullptr && depth_resource != nullptr) {
        instance->SetUpD3D12(mvec_resource->GetDesc(), depth_resource->GetDesc());
    }

    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        return original_fn(afr_viewport_handle, tags, numTags, cmdBuffer);
    }
    auto result = original_fn(viewport, tags, numTags, cmdBuffer);
    return result;
}

sl::Result NvidiaInterposterExperiment::on_slEvaluateFeature(sl::Feature feature, const sl::FrameToken& frame, sl::BaseStructure** inputs, uint32_t numInputs, sl::CommandBuffer* cmdBuffer)
{
    auto instance    = NvidiaInterposterExperiment::Get();
    auto original_fn = instance->m_evaluate_feature_hook->get_original<decltype(NvidiaInterposterExperiment::on_slEvaluateFeature)>();
    auto vr          = VR::get();
    if (numInputs != 1) {
        spdlog::error("numInputs != 1");
    }
//    auto get_frame_token_fn = instance->m_get_new_frame_token_hook->get_original<decltype(DlssDualView::on_slGetNewFrameToken)>();
//    uint32_t currentFrame = frame;
//    auto half_frame = currentFrame / 2;
//    sl::FrameToken* new_token;
//    get_frame_token_fn(new_token, &half_frame);
    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT && feature == sl::kFeatureDLSS) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        sl::BaseStructure* afr_inputs[] = {&afr_viewport_handle};
        return original_fn(feature, frame, afr_inputs, 1, cmdBuffer);
    }
    auto result = original_fn(feature, frame, inputs, numInputs, cmdBuffer);
    return result;
}

// TODO vr->get_current_render_eye() == VRRuntime::Eye::RIGHT in real is left eye texture
sl::Result NvidiaInterposterExperiment::on_slSetConstants(sl::Constants& values, const sl::FrameToken& frame, sl::ViewportHandle& viewport)
{
    auto instance = NvidiaInterposterExperiment::Get();

    static bool dynamic_set_options_hooked = false;
    if (!dynamic_set_options_hooked) {
        //        REL::Relocation<uintptr_t*> dlssSetOptionsFnaddr{REL::ID(1171973)};
        REL::Relocation<uintptr_t*> dlssSetOptionsFnaddr{ RE2::MemoryOffsets::Streamline::SetDlssOptions() };
        if (*dlssSetOptionsFnaddr.get() != 0) {
            instance->m_dlss_set_options_hook = std::make_unique<PointerHook>((void**)dlssSetOptionsFnaddr.get(), (void*)&NvidiaInterposterExperiment::on_dlssSetOptions);
            spdlog::info("Hooked dlssSetOptions");
            dynamic_set_options_hooked = true;
        }
        else {
            spdlog::error("DLSS not yet initialized");
        }
    }
//    spdlog::info("slSetConstants called cmaeraMotionincl={} mvecscale={}x{}", (UINT)values.cameraMotionIncluded, values.mvecScale.x, values.mvecScale.y);
    g_dlss_constants.mvecScale = { values.mvecScale.x, values.mvecScale.y };
    memcpy(&g_dlss_constants.clipToPrevClip, &values.clipToPrevClip, sizeof(float) * 16);
    auto original_fn = instance->m_set_constants_hook->get_original<decltype(NvidiaInterposterExperiment::on_slSetConstants)>();
    auto vr          = VR::get();
//    auto get_frame_token_fn = instance->m_get_new_frame_token_hook->get_original<decltype(DlssDualView::on_slGetNewFrameToken)>();
//    uint32_t currentFrame = frame;
//    auto half_frame = currentFrame / 2;
//    sl::FrameToken* new_token;
//    get_frame_token_fn(new_token, &half_frame);
    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        return original_fn(values, frame, afr_viewport_handle);
    }
    auto result = original_fn(values, frame, viewport);
    return result;
}

uint64_t NvidiaInterposterExperiment::on_ceGetNewFrameToken(uintptr_t a1, uint32_t frameIndex)
{
    auto instance    = NvidiaInterposterExperiment::Get();
    auto original_fn = instance->m_onGetDllsFrameToken->get_original<decltype(NvidiaInterposterExperiment::on_ceGetNewFrameToken)>();

    auto result = original_fn(a1, frameIndex);
    return result;
}

//
// sl::FrameToken* DlssDualView::get_half_frame_internal(const sl::FrameToken& token)
//{
//    auto instance = DlssDualView::Get();
//    uint32_t frameIndex = token;
//    auto half_frame = frameIndex/2;
//    auto original_fn = instance->m_get_new_frame_token_hook->get_original<decltype(DlssDualView::on_slGetNewFrameToken)>();
//    sl::FrameToken* new_token = nullptr;
//    original_fn(new_token, &half_frame);
//    return new_token;
//}

sl::Result NvidiaInterposterExperiment::on_dlssSetOptions(const sl::ViewportHandle& viewport, const sl::DLSSOptions& options)
{
    auto     instance         = NvidiaInterposterExperiment::Get();
    auto     original_fn      = instance->m_dlss_set_options_hook->get_original<decltype(NvidiaInterposterExperiment::on_dlssSetOptions)*>();
    uint32_t viewport_id      = viewport;
    instance->afr_viewport_id = viewport_id + 1;
    auto vr                   = VR::get();
    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        return original_fn(afr_viewport_handle, options);
    }
    return original_fn(viewport, options);
}

void NvidiaInterposterExperiment::SetUpD3D12(const D3D12_RESOURCE_DESC& mvec_desc, const D3D12_RESOURCE_DESC& depth_desc)
{
    if (d3d12_inited.load()) {
        auto current_mvec  = mvec_resource_copy->GetDesc();
        auto current_depth = depth_resource_copy->GetDesc();
        if (current_mvec.Width != mvec_desc.Width || current_mvec.Height != mvec_desc.Height || current_mvec.Format != mvec_desc.Format || current_depth.Width != depth_desc.Width
            || current_depth.Height != depth_desc.Height || current_depth.Format != depth_desc.Format)
        {
            std::scoped_lock _{mtx};
            if(d3d12_inited.exchange(false)) {
                ResetD3D12();
            }
        }
    }
    if (!d3d12_inited.load()) {
        std::scoped_lock _{mtx};
        if(d3d12_inited.load()) {
            return;
        }
        spdlog::info("Setting up DLSS debug layer D3D12");
        auto device = g_framework->get_d3d12_hook()->get_device();
        m_cmd_ctx.setup(L"DLSS Debug Layer");

        D3D12_HEAP_PROPERTIES heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        try {
            m_srv_heap = std::make_unique<DirectX::DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, (UINT)SRV::COUNT);
        }
        catch (...) {
            spdlog::error("Failed to create SRV/RTV descriptor heap for DLSS");
            d3d12_inited.exchange(false);
            return;
        }

        {
            auto mvec_desc_copy = mvec_desc;
            mvec_desc_copy.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            mvec_desc_copy.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &mvec_desc_copy, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                       IID_PPV_ARGS(mvec_resource_copy.GetAddressOf()))))
            {
                spdlog::error("[VR] Failed to create mvec copy copy for DLSS.");
                d3d12_inited.exchange(false);
                return;
            }
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format                        = DXGI_FORMAT_R16G16_FLOAT;
            srv_desc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Texture2D.MipLevels           = 1;
            srv_desc.Texture2D.MostDetailedMip     = 0;
            srv_desc.Texture2D.PlaneSlice          = 0;
            srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
            device->CreateShaderResourceView(mvec_resource_copy.Get(), &srv_desc, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_MVEC));
        }

        {
            auto depth_desc_copy   = depth_desc;
            depth_desc_copy.Format = DXGI_FORMAT_R32_TYPELESS;
            depth_desc_copy.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            depth_desc_copy.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            D3D12_CLEAR_VALUE clearValue = {};
            clearValue.Format = DXGI_FORMAT_R32_FLOAT;
            if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &depth_desc_copy, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue,
                                                       IID_PPV_ARGS(depth_resource_copy.GetAddressOf()))))
            {
                spdlog::error("[VR] Failed to create depth copy copy for DLSS.");
                d3d12_inited.exchange(false);
                return;
            }
            if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &depth_desc_copy, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue,
                                                       IID_PPV_ARGS(depth_resource_copy_1.GetAddressOf()))))
            {
                spdlog::error("[VR] Failed to create depth copy copy for DLSS.");
                d3d12_inited.exchange(false);
                return;
            }
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format                        = DXGI_FORMAT_R32_FLOAT;
            srv_desc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Texture2D.MipLevels           = 1;
            srv_desc.Texture2D.MostDetailedMip     = 0;
            srv_desc.Texture2D.PlaneSlice          = 0;
            srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
            device->CreateShaderResourceView(depth_resource_copy.Get(), &srv_desc, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_DEPTH));
            device->CreateShaderResourceView(depth_resource_copy_1.Get(), &srv_desc, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_DEPTH_1));
        }

        spdlog::info("[VR] DLSS debug layer D3D12 resrouce setup complete.");

        if (!CreateRootSignature(device) || !CreatePipelineStates(device)) {
            spdlog::error("[VR] Failed to create DLSS root signature.");
            d3d12_inited.exchange(false);
            return;
        }
        d3d12_inited.exchange(true);
    }
}

bool NvidiaInterposterExperiment::CreateRootSignature(ID3D12Device* device)
{
    HRESULT                   hr;
    CD3DX12_DESCRIPTOR_RANGE1 descriptorRange[1];
    CD3DX12_ROOT_PARAMETER1   rootParams[2];

    descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, (UINT)SRV::COUNT, 0, 0);
    rootParams[0].InitAsDescriptorTable(1, &descriptorRange[0], D3D12_SHADER_VISIBILITY_PIXEL);
    ;
    rootParams[1].InitAsConstants(22, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
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
    spdlog::info("[VR] DLSS root signature created.");
    return true;
}

bool NvidiaInterposterExperiment::CreatePipelineStates(ID3D12Device* device)
{
    // debug mvec pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC debugMvec_pso = {};
        debugMvec_pso.InputLayout                        = {};
        debugMvec_pso.pRootSignature                     = m_rootSignature.Get();
        debugMvec_pso.VS                                 = CD3DX12_SHADER_BYTECODE(&SHADERS::DLSS::DEBUG::VS::g_vs_main, sizeof(SHADERS::DLSS::DEBUG::VS::g_vs_main));
        debugMvec_pso.PS                                 = CD3DX12_SHADER_BYTECODE(&SHADERS::DLSS::DEBUG::PS::g_ps_main, sizeof(SHADERS::DLSS::DEBUG::PS::g_ps_main));

        debugMvec_pso.BlendState                    = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        debugMvec_pso.DepthStencilState             = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        debugMvec_pso.DepthStencilState.DepthEnable = false;
        debugMvec_pso.SampleMask                    = UINT_MAX;
        debugMvec_pso.RasterizerState               = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        debugMvec_pso.PrimitiveTopologyType         = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        debugMvec_pso.NumRenderTargets              = 1;
        debugMvec_pso.DSVFormat                     = DXGI_FORMAT_UNKNOWN;
        debugMvec_pso.RTVFormats[0]                 = DXGI_FORMAT_R8G8B8A8_UNORM;
        debugMvec_pso.SampleDesc.Count              = 1;
        debugMvec_pso.SampleDesc.Quality            = 0;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&debugMvec_pso, IID_PPV_ARGS(visualize_mved_pso.GetAddressOf())));
        spdlog::info("[VR] debug mvec pso.");
    }
    return true;
}

void NvidiaInterposterExperiment::Apply(ID3D12GraphicsCommandList* commandList, ID3D12Resource* backbuffer, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle)
{
    if (g_dlss_settings.debugOutput != 1) {
        return;
    }

    D3D12_RESOURCE_BARRIER barriers[1]{
        CD3DX12_RESOURCE_BARRIER::Transition(backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET),
    };

    const auto render_target_desc = backbuffer->GetDesc();
    float      clear_color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

    D3D12_VIEWPORT viewport{};
    viewport.Width    = (float)render_target_desc.Width;
    viewport.Height   = (float)render_target_desc.Height;
    viewport.MinDepth = D3D12_MIN_DEPTH;
    viewport.MaxDepth = D3D12_MAX_DEPTH;

    D3D12_RECT scissor_rect{};
    scissor_rect.left   = 0;
    scissor_rect.top    = 0;
    scissor_rect.right  = (LONG)render_target_desc.Width;
    scissor_rect.bottom = (LONG)render_target_desc.Height;

    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissor_rect);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetGraphicsRoot32BitConstants(1, 22, &g_dlss_constants, 0);
    ID3D12DescriptorHeap* descriptorHeaps[] = { m_srv_heap->Heap() };
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    commandList->SetGraphicsRootDescriptorTable(0, m_srv_heap->GetFirstGpuHandle());

    {
        commandList->SetPipelineState(visualize_mved_pso.Get());
        commandList->ClearRenderTargetView(*rtv_handle, clear_color, 1, &scissor_rect);
        commandList->OMSetRenderTargets(1, rtv_handle, FALSE, nullptr);
        commandList->DrawInstanced(6, 1, 0, 0);
    }

    barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
    commandList->ResourceBarrier(1, barriers);
}

sl::Result NvidiaInterposterExperiment::on_slFreeResources(sl::Feature feature, const sl::ViewportHandle& viewport)
{
    return sl::Result::eErrorMissingOrInvalidAPI;
}

sl::Result NvidiaInterposterExperiment::on_slAllocateResources(sl::CommandBuffer* cmdBuffer, sl::Feature feature, const sl::ViewportHandle& viewport)
{
    auto instance    = NvidiaInterposterExperiment::Get();
    auto original_fn = instance->m_allocate_resources_hook->get_original<decltype(NvidiaInterposterExperiment::on_slAllocateResources)>();
    uint32_t viewport_id      = viewport;
    instance->afr_viewport_id = viewport_id + 1;
    return original_fn(cmdBuffer, feature, viewport);
}
