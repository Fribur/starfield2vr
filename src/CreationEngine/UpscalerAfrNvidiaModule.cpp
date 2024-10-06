//
// Created by sergp on 8/18/2024.
//

#include "UpscalerAfrNvidiaModule.h"
#include "sl_matrix_helpers.h"
#include <CreationEngine/RE2/offsets.h>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <mods/VR.hpp>
#include "REL/Relocation.h"


//DlssConstants g_dlss_constants{};
//DlssSettings  g_dlss_settings{};
//DlssDebugInfo g_dlss_debug_info{};

void UpscalerAfrNvidiaModule::InstallHooks()
{
    static HMODULE p_hm_sl_interposter = nullptr;
    if (p_hm_sl_interposter) {
        return;
    }
    spdlog::info("Installing sl.interposer hooks");
    while ((p_hm_sl_interposter = GetModuleHandle("sl.interposer.dll")) == nullptr) {
        std::this_thread::yield();
    }

//    auto getNewFrameTokenFn = GetProcAddress(g_interposer, "slGetNewFrameToken");
//    m_get_new_frame_token_hook = std::make_unique<FunctionHook>((void **)getNewFrameTokenFn, (void *) &DlssDualView::on_slGetNewFrameToken);
//    if(!m_get_new_frame_token_hook->create()) {
//        spdlog::error("Failed to hook slGetNewFrameToken");
//    }

    auto slSetTagFn = GetProcAddress(p_hm_sl_interposter, "slSetTag");
    m_set_tag_hook  = std::make_unique<FunctionHook>((void**)slSetTagFn, (void*)&UpscalerAfrNvidiaModule::on_slSetTag);
    if (!m_set_tag_hook->create()) {
        spdlog::error("Failed to hook slSetTag");
    }

    auto slEvaluateFeatureFn = GetProcAddress(p_hm_sl_interposter, "slEvaluateFeature");
    m_evaluate_feature_hook  = std::make_unique<FunctionHook>((void**)slEvaluateFeatureFn, (void*)&UpscalerAfrNvidiaModule::on_slEvaluateFeature);
    if (!m_evaluate_feature_hook->create()) {
        spdlog::error("Failed to hook slEvaluateFeature");
    }

    auto slSetConstantsFn = GetProcAddress(p_hm_sl_interposter, "slSetConstants");
    m_set_constants_hook  = std::make_unique<FunctionHook>((void**)slSetConstantsFn, (void*)&UpscalerAfrNvidiaModule::on_slSetConstants);
    if (!m_set_constants_hook->create()) {
        spdlog::error("Failed to hook slSetConstants");
    }

    //TODO why it is commented out
//    auto slAllocateResourcesFn = GetProcAddress(p_hm_sl_interposter, "slAllocateResources");
//    m_allocate_resources_hook  = std::make_unique<FunctionHook>((void**)slAllocateResourcesFn, (void*)&UpscalerAfrNvidiaModule::on_slAllocateResources);
//    if (!m_allocate_resources_hook->create()) {
//        spdlog::error("Failed to hook slAllocateResources");
//    }
//
//    auto slFreeResourcesFn = GetProcAddress(p_hm_sl_interposter, "slFreeResources");
//    m_free_resources_hook  = std::make_unique<FunctionHook>((void**)slFreeResourcesFn, (void*)&UpscalerAfrNvidiaModule::on_slFreeResources);
//    if (!m_free_resources_hook->create()) {
//        spdlog::error("Failed to hook slFreeResources");
//    }

//        REL::Relocation<uintptr_t>  getDllsFrameTokenFnAddr{REL::ID(1078687)};
//        m_onGetDllsFrameToken = std::make_unique<PolyHook2FunctionHook>(getDllsFrameTokenFnAddr.address(), (uintptr_t)&DlssDualView::on_ceGetNewFrameToken);
//        m_onGetDllsFrameToken->create();

    spdlog::info("sl.interposer Hooks installed");
}

/*sl::Result UpscalerAfrNvidiaModule::on_slGetNewFrameToken(sl::FrameToken*& token, const uint32_t* frameIndex)
{
    auto instance = UpscalerAfrNvidiaModule::Get();

    auto original_fn = instance->m_get_new_frame_token_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slGetNewFrameToken)>();
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
}*/

sl::Result UpscalerAfrNvidiaModule::on_slSetTag(sl::ViewportHandle& viewport, const sl::ResourceTag* tags, uint32_t numTags, sl::CommandBuffer* cmdBuffer)
{
    auto            instance    = UpscalerAfrNvidiaModule::Get();
    auto            original_fn = instance->m_set_tag_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slSetTag)>();
    auto            vr          = VR::get();
    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        return original_fn(afr_viewport_handle, tags, numTags, cmdBuffer);
    }
    auto result = original_fn(viewport, tags, numTags, cmdBuffer);
    return result;
}

sl::Result UpscalerAfrNvidiaModule::on_slEvaluateFeature(sl::Feature feature, const sl::FrameToken& frame, sl::BaseStructure** inputs, uint32_t numInputs, sl::CommandBuffer* cmdBuffer)
{
    auto instance    = UpscalerAfrNvidiaModule::Get();
    auto original_fn = instance->m_evaluate_feature_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slEvaluateFeature)>();
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
sl::Result UpscalerAfrNvidiaModule::on_slSetConstants(sl::Constants& values, const sl::FrameToken& frame, sl::ViewportHandle& viewport)
{
    auto instance = UpscalerAfrNvidiaModule::Get();

    static bool dynamic_set_options_hooked = false;
    if (!dynamic_set_options_hooked) {
        //        REL::Relocation<uintptr_t*> dlssSetOptionsFnaddr{REL::ID(1171973)};
        REL::Relocation<uintptr_t*> dlssSetOptionsFnaddr{ RE2::MemoryOffsets::Streamline::SetDlssOptions() };
        if (*dlssSetOptionsFnaddr.get() != 0) {
            instance->m_dlss_set_options_hook = std::make_unique<PointerHook>((void**)dlssSetOptionsFnaddr.get(), (void*)&UpscalerAfrNvidiaModule::on_dlssSetOptions);
            spdlog::info("Hooked dlssSetOptions");
            dynamic_set_options_hooked = true;
        }
        else {
            spdlog::error("DLSS not yet initialized");
        }
    }
//    spdlog::info("slSetConstants called cmaeraMotionincl={} mvecscale={}x{}", (UINT)values.cameraMotionIncluded, values.mvecScale.x, values.mvecScale.y);
    auto original_fn = instance->m_set_constants_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slSetConstants)>();
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
//
//uint64_t UpscalerAfrNvidiaModule::on_ceGetNewFrameToken(uintptr_t a1, uint32_t frameIndex)
//{
//    auto instance    = UpscalerAfrNvidiaModule::Get();
//    auto original_fn = instance->m_onGetDllsFrameToken->get_original<decltype(UpscalerAfrNvidiaModule::on_ceGetNewFrameToken)>();
//
//    auto result = original_fn(a1, frameIndex);
//    return result;
//}

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

sl::Result UpscalerAfrNvidiaModule::on_dlssSetOptions(const sl::ViewportHandle& viewport, const sl::DLSSOptions& options)
{
    auto     instance         = UpscalerAfrNvidiaModule::Get();
    auto     original_fn      = instance->m_dlss_set_options_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_dlssSetOptions)*>();
    uint32_t viewport_id      = viewport;
    instance->afr_viewport_id = viewport_id + 1;
    auto vr                   = VR::get();
    if(vr->get_current_render_eye() == VRRuntime::Eye::RIGHT) {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        return original_fn(afr_viewport_handle, options);
    }
    return original_fn(viewport, options);
}

sl::Result UpscalerAfrNvidiaModule::on_slFreeResources(sl::Feature feature, const sl::ViewportHandle& viewport)
{
    auto instance    = UpscalerAfrNvidiaModule::Get();
    auto original_fn = instance->m_free_resources_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slFreeResources)>();
    {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        original_fn(feature, afr_viewport_handle);
    }
    return original_fn(feature, viewport);
}

sl::Result UpscalerAfrNvidiaModule::on_slAllocateResources(sl::CommandBuffer* cmdBuffer, sl::Feature feature, const sl::ViewportHandle& viewport)
{
    auto instance    = UpscalerAfrNvidiaModule::Get();
    auto original_fn = instance->m_allocate_resources_hook->get_original<decltype(UpscalerAfrNvidiaModule::on_slAllocateResources)>();
    uint32_t viewport_id      = viewport;
    instance->afr_viewport_id = viewport_id + 1;
    {
        sl::ViewportHandle afr_viewport_handle{instance->afr_viewport_id};
        original_fn(cmdBuffer, feature, afr_viewport_handle);
    }
    return original_fn(cmdBuffer, feature, viewport);
}
