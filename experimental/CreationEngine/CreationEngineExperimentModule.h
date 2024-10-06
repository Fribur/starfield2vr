#ifndef USE_STARFIELD_SDK_LITE

#pragma once
#include <RE/C/CreationRendererPrivate.h>
#include <shared/sdk/Math.hpp>
#include <utils/PolyHook2FunctionHook.h>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <windef.h>
#include "CreationEngineRendererModule.h"

using Microsoft::WRL::ComPtr;


class CreationEngineExperimentModule
{
public:
    void InstallHooks();

    static inline CreationEngineExperimentModule* Get()
    {
        static auto instance(new CreationEngineExperimentModule);
        return instance;
    }

    [[nodiscard]] inline RE::CreationEngineSettings* GetCreationEngineSettings() const
    {
        return *m_creationEngineSettings;
    }

    static inline int GetGlobalFrameCount()
    {
        auto pFrameCount = Get()->m_globalFrameCount;
        return pFrameCount == nullptr? 0: *pFrameCount;
    }

    void SwapBuffer(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* originalBuffer, int index, int sourceSlot, int destSlot);

    static inline void CopyResource(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* pSrcResource, ID3D12Resource* pDstResource, D3D12_RESOURCE_STATES srcState, D3D12_RESOURCE_STATES dstState)
    {
        D3D12_RESOURCE_BARRIER barriers[2]
            = { CD3DX12_RESOURCE_BARRIER::Transition(pSrcResource, srcState, D3D12_RESOURCE_STATE_COPY_SOURCE),
                CD3DX12_RESOURCE_BARRIER::Transition(pDstResource, dstState, D3D12_RESOURCE_STATE_COPY_DEST) };
        cmdList->ResourceBarrier(2, barriers);
        cmdList->CopyResource(pDstResource, pSrcResource);
        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[0].Transition.StateAfter  = srcState;
        barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[1].Transition.StateAfter  = dstState;
        cmdList->ResourceBarrier(2, barriers);
    }
    uintptr_t onPipelineGenerationStartedEvent(uintptr_t i, uintptr_t i1);

    uintptr_t onPipelineGenerationStartedEvent2(uintptr_t i);

    uintptr_t onPipelineGenerationStartedEventLambda(uintptr_t i);

    uintptr_t onRenderPassVfunc1(RE::CreationRendererPrivate::RenderPass* pass, va_list args);
    uintptr_t onRenderPassVfunc2(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i, uintptr_t i1, uintptr_t i2, uintptr_t i3);
    uintptr_t onRenderPassVfunc3(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i, uintptr_t i1);
    uintptr_t onRenderPassVfunc4(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i);

    void onSetSlDlssOptions(__int64 i, __int64 i1, RE::RenderPassConstantBufferView* pView, __int64 i3, unsigned int i4, bool b);

    void onStartRenderPipeline(__int64 i, __int64 i1);

    __int64 onVFunc9MainRenderGraph(RE::CreationRendererPrivate::MainRenderGraph* graph, __int64 i, __int64 i1, __int64 i2);

    __int64 onVFunc8FrameRenderGraph(RE::CreationRendererPrivate::RenderGraph* pGraph, __int64 i);

    __int64 onVFunc9FrameRenderGraph(RE::CreationRendererPrivate::RenderGraph* pGraph, __int64 i);

    __int64 onVFunc10MainRenderGraph(RE::CreationRendererPrivate::MainRenderGraph* pGraph, __int64 i, __int64 i1, __int64 i2);

    __int64 onRenderGraphRenderStart(RE::CreationRendererPrivate::RenderGraph* pGraph, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, __int64 i1, __int64 i2);

    __int64 onUnk2MainRenderGraph(void* pGraph, __int64 i, __int64 i1, __int64 i2);

    __int64 onUnk3MainRenderGraph(void* pVoid, __int64 i, __int64 i1, __int64 i2);

    __int64 onRenderFrameStart(void* pVoid, __int64 i, __int64 i1, __int64 i2);

    void onRenderFrameEnd(__int64 i, __int64 i1, __int64 i2, __int64 i3);

    void onRenderFrameEnd2(__int64 i, __int64 i1, __int64 i2, __int64 i3);
    void SetWindowSize(int width, int height);

    __int64 onVFunc7RenderGraph(RE::CreationRendererPrivate::MainRenderGraph* pGraph, __int64 i, __int64 i1, __int64 i2);

    uintptr_t onDllsProduceInput(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1);

    uintptr_t onDllsUpscale(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1);

    uintptr_t onRenderPassVfunc8MotionVectorComposite(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1);

    uintptr_t onUpdateConstantBufferView(uint8_t i, uint8_t i1, uintptr_t i2, uintptr_t i3, uintptr_t i4, double d, char i5, RE::RenderPassConstantBufferView* pView);

    uintptr_t onSetMotionVectorPassConstants(uintptr_t i, RE::RenderPassConstantBufferView* pView, RE::MotionVectorRenderPassConstants* constants, uintptr_t renderPassData);

private:
    CreationEngineExperimentModule()  = default;
    ~CreationEngineExperimentModule() = default;
    std::unique_ptr<PolyHook2FunctionHook> m_onPipelineGenerationStartedEventHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onPipelineGenerationStartedEventHook2;
    std::unique_ptr<PolyHook2FunctionHook> m_onPipelineGenerationStartedEventLambdaHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc1RenderPassHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc2RenderPassHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc3RenderPassHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc4RenderPassHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onSetDlssOptions;
    std::unique_ptr<PolyHook2FunctionHook> m_OnStartRenderPipelineHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc9RenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc10RenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onRenderGraphRenderStartHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onUnk2RenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onUnk3RenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc7RenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc8RenderPassDlssProduceInputsHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc8RenderPassDlssUpscaleHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc8RenderPassMotionVectorCompositeHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc8FrameRenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onVfunc9FrameRenderGraphHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onRenderFrameStartHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onRenderFrameEndHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onRenderFrameEndHook2;
    std::unique_ptr<PolyHook2FunctionHook> m_onSetWindoSizeHook;

    std::unique_ptr<PolyHook2FunctionHook> m_onUpdateConstantBufferViewHook;
    std::unique_ptr<PolyHook2FunctionHook> m_onSetMotionVectorPassConstantsHook;

    RE::CreationRendererPrivate::RenderPass* m_startFramePass{ nullptr};
    RE::CreationRendererPrivate::RenderPass* m_endFramePass{ nullptr};
    RE::CreationRendererPrivate::RenderPass* m_framePass{ nullptr};
    RE::CreationEngineSettings** m_creationEngineSettings{ nullptr};
    std::mutex m_last_resolution_mutex{};
    std::chrono::steady_clock::time_point m_last_resolution_sync{};

    int indexAfterPresent{0};

    int* m_globalFrameCount{ nullptr };

    ComPtr<ID3D12Resource> m_pastBuffer[12][4];


    void RenderGraphStart(RE::CreationRendererPrivate::RenderGraph* pGraph, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, bool before);
    static bool ValidateResource(ID3D12Resource* source, ComPtr<ID3D12Resource> pPtr[4]);
};

#endif