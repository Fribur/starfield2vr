#pragma once

#include <DescriptorHeap.h>
#include <d3d12.h>
#include <mods/vr/d3d12/ComPtr.hpp>
#include <mods/vr/d3d12/CommandContext.hpp>
#include <sl.h>
#include <sl_dlss.h>
#include <utility/PointerHook.hpp>
#include <utils/FunctionHook.hpp>
#include <utils/PolyHook2FunctionHook.h>
using Microsoft::WRL::ComPtr;

class NvidiaInterposterExperiment
{
public:
    inline static NvidiaInterposterExperiment* Get()
    {
        static auto instance(new NvidiaInterposterExperiment);
        return instance;
    }
    inline void ResetD3D12() {
        if(d3d12_inited.exchange(false)) {
            spdlog::info("Resetting D3D12 resources for DLSS Dual View");
            m_cmd_ctx.reset();
            visualize_mved_pso.Reset();

            m_rootSignature.Reset();

            m_rtv_heap.reset();
            m_dsv_heap.reset();
            m_srv_heap.reset();

            mvec_resource_copy.Reset();
            depth_resource_copy.Reset();
            depth_resource_copy_1.Reset();
        }
    }
    inline bool isD3D12Inited() {
        return d3d12_inited.load();
    }
    inline ID3D12Resource* GetMvecResourceCopy() {
        return mvec_resource_copy.Get();
    }
    inline ID3D12Resource* GetDepthResourceCopy() {
        return depth_resource_copy.Get();
    }

    void SetUpD3D12(const D3D12_RESOURCE_DESC& mvec_desc, const D3D12_RESOURCE_DESC& depth_desc);
    void Apply(ID3D12GraphicsCommandList* cmdLis,ID3D12Resource* backbuffer, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle);
private:
    void InstallHooks();
    inline NvidiaInterposterExperiment() {
        InstallHooks();
    }
    ~NvidiaInterposterExperiment() = default;

    std::unique_ptr<FunctionHook> m_get_new_frame_token_hook{nullptr};
    std::unique_ptr<FunctionHook> m_set_tag_hook{nullptr};
    std::unique_ptr<FunctionHook> m_evaluate_feature_hook{nullptr};
    std::unique_ptr<FunctionHook> m_set_constants_hook{nullptr};
    std::unique_ptr<FunctionHook> m_allocate_resources_hook{nullptr};
    std::unique_ptr<FunctionHook> m_free_resources_hook{nullptr};

    std::unique_ptr<PointerHook> m_dlss_set_options_hook{nullptr};

    std::atomic<bool> d3d12_inited{false};

    std::unique_ptr<PolyHook2FunctionHook> m_onGetDllsFrameToken{nullptr};

    uint32_t afr_viewport_id{1};

    static sl::Result on_slGetNewFrameToken(sl::FrameToken*& token, const uint32_t* frameIndex = nullptr);
    static sl::Result on_slSetTag(sl::ViewportHandle& viewport, const sl::ResourceTag* tags, uint32_t numTags, sl::CommandBuffer* cmdBuffer);
    static sl::Result on_slEvaluateFeature(sl::Feature feature, const sl::FrameToken& frame, sl::BaseStructure** inputs, uint32_t numInputs, sl::CommandBuffer* cmdBuffer);
    static sl::Result on_slSetConstants(sl::Constants& values, const sl::FrameToken& frame, sl::ViewportHandle& viewport);
    static sl::Result on_dlssSetOptions(const sl::ViewportHandle& viewport, const sl::DLSSOptions& options);
    static sl::Result on_slAllocateResources(sl::CommandBuffer* cmdBuffer, sl::Feature feature, const sl::ViewportHandle& viewport);
    static sl::Result on_slFreeResources(sl::Feature feature, const sl::ViewportHandle& viewport);
    static sl::FrameToken* get_half_frame_internal(const sl::FrameToken& token);

    static uint64_t on_ceGetNewFrameToken(uintptr_t  a1, uint32_t frameIndex);


    bool CreateRootSignature(ID3D12Device* device);
    bool CreatePipelineStates(ID3D12Device* device);

    //D3D12 experiments
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> visualize_mved_pso;

    std::unique_ptr<DirectX::DescriptorHeap>  m_dsv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_srv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_rtv_heap{};

    ComPtr<ID3D12Resource> mvec_resource_copy;
    ComPtr<ID3D12Resource> depth_resource_copy;
    ComPtr<ID3D12Resource> depth_resource_copy_1;


    d3d12::CommandContext m_cmd_ctx{};

    enum class SRV : int {
        DEBUG_MVEC,
        DEBUG_DEPTH,
        DEBUG_DEPTH_1,
        DEBUG_REPROJECT_DEPTH,
        COUNT
    };

    enum class RTV : int {
        MAIN_RT,
        COUNT
    };
    std::recursive_mutex mtx{};

};
