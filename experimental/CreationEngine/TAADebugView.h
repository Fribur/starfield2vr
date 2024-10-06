#pragma once

#include <DescriptorHeap.h>
#include <d3d12.h>
#include <mods/vr/d3d12/ComPtr.hpp>
#include <mods/vr/d3d12/CommandContext.hpp>
#include <RE/C/CreationRendererPrivate.h>
#include <utils/PolyHook2FunctionHook.h>
using Microsoft::WRL::ComPtr;

class TAADebugView
{
public:
    inline static TAADebugView* Get()
    {
        static auto instance(new TAADebugView);
        return instance;
    }
    inline void Reset() {
        inited.exchange(false);
//        m_cmd_ctx.reset();
        visualize_resource_pso.Reset();

        m_rootSignature.Reset();

        m_rtv_heap.reset();
        m_dsv_heap.reset();
        m_srv_heap.reset();

        resource_copy.Reset();
        resource_comp.Reset();

    }
    void Apply(ID3D12GraphicsCommandList* cmdLis,ID3D12Resource* backbuffer, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle);
    void Init(ID3D12Device* device);
private:
    enum class SRV : int {
        DEBUG_RESOURCE,
        DEBUG_RESOURCE_COMP,
        COUNT
    };

    enum class RTV : int {
        MAIN_RT,
        COUNT
    };

    void InitResources(const D3D12_RESOURCE_DESC& desc, ComPtr<ID3D12Resource>& destination, SRV srv);
    void InstallHooks();
    inline TAADebugView() {
        InstallHooks();
    }
    ~TAADebugView() = default;

    std::unique_ptr<PolyHook2FunctionHook> taa_vfunc7_hook{nullptr};

    static uintptr_t onTaaPass(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1);

    bool CreateRootSignature(ID3D12Device* device);
    bool CreatePipelineStates(ID3D12Device* device);

    //D3D12 experiments
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> visualize_resource_pso;

    std::unique_ptr<DirectX::DescriptorHeap>  m_dsv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_srv_heap{};
    std::unique_ptr<DirectX::DescriptorHeap>  m_rtv_heap{};

    ComPtr<ID3D12Resource> resource_copy;
    ComPtr<ID3D12Resource> resource_comp;


//    d3d12::CommandContext m_cmd_ctx{};


    std::atomic<bool> inited{false};
    std::recursive_mutex mtx{};

};
