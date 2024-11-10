#pragma once

#include <DescriptorHeap.h>
#include <d3d12.h>
#include <mods/vr/d3d12/ComPtr.hpp>
#include <mods/vr/d3d12/CommandContext.hpp>
#include <sl.h>
#include <sl_dlss.h>
#include <utility/PointerHook.hpp>
#include <utils/FunctionHook.h>

class UpscalerAfrNvidiaModule
{
public:
    inline static UpscalerAfrNvidiaModule* Get()
    {
        static auto instance(new UpscalerAfrNvidiaModule);
        return instance;
    }

private:
    void InstallHooks();
    inline UpscalerAfrNvidiaModule() {
        InstallHooks();
    }
    ~UpscalerAfrNvidiaModule() = default;

    std::unique_ptr<FunctionHook> m_get_new_frame_token_hook{nullptr};
    std::unique_ptr<FunctionHook> m_set_tag_hook{nullptr};
    std::unique_ptr<FunctionHook> m_evaluate_feature_hook{nullptr};
    std::unique_ptr<FunctionHook> m_set_constants_hook{nullptr};
    std::unique_ptr<FunctionHook> m_allocate_resources_hook{nullptr};
    std::unique_ptr<FunctionHook> m_free_resources_hook{nullptr};

    std::unique_ptr<PointerHook> m_dlss_set_options_hook{nullptr};


    std::unique_ptr<FunctionHook> m_onGetDllsFrameToken{nullptr};

    uint32_t afr_viewport_id{1};

//    static sl::Result on_slGetNewFrameToken(sl::FrameToken*& token, const uint32_t* frameIndex = nullptr);
    static sl::Result on_slSetTag(sl::ViewportHandle& viewport, const sl::ResourceTag* tags, uint32_t numTags, sl::CommandBuffer* cmdBuffer);
    static sl::Result on_slEvaluateFeature(sl::Feature feature, const sl::FrameToken& frame, sl::BaseStructure** inputs, uint32_t numInputs, sl::CommandBuffer* cmdBuffer);
    static sl::Result on_slSetConstants(sl::Constants& values, const sl::FrameToken& frame, sl::ViewportHandle& viewport);
    static sl::Result on_dlssSetOptions(const sl::ViewportHandle& viewport, const sl::DLSSOptions& options);
    static sl::Result on_slAllocateResources(sl::CommandBuffer* cmdBuffer, sl::Feature feature, const sl::ViewportHandle& viewport);
    static sl::Result on_slFreeResources(sl::Feature feature, const sl::ViewportHandle& viewport);
//    static sl::FrameToken* get_half_frame_internal(const sl::FrameToken& token);

//    static uint64_t on_ceGetNewFrameToken(uintptr_t  a1, uint32_t frameIndex);
};
