//
// Created by sergp on 6/26/2024.
//

#ifndef USE_STARFIELD_SDK_LITE
#include "CreationEngineExperimentModule.h"
#include "CreationEngineGameModule.h"
#include "NvidiaInterposterExperiment.h"
#include <CreationEngine/RE2/offsets.h>
#include <RE/MultiCamervaViewModule.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <mods/VR.hpp>

uintptr_t onPipelineGenerationStartedEventDetour(uintptr_t m_message_processor, uintptr_t a_event) {
    return CreationEngineExperimentModule::Get()->onPipelineGenerationStartedEvent(m_message_processor, a_event);
}

uintptr_t onPipelineGenerationStartedEvent2Detour(uintptr_t m_message_processor) {
    return CreationEngineExperimentModule::Get()->onPipelineGenerationStartedEvent2(m_message_processor);
}

uintptr_t onPipelineGenerationStartedEventLambdaDetour(uintptr_t a_event) {
    return CreationEngineExperimentModule::Get()->onPipelineGenerationStartedEventLambda(a_event);
}


uintptr_t onRenderPassVfunc1Detour(RE::CreationRendererPrivate::RenderPass* a_renderPass, va_list args) {
    return CreationEngineExperimentModule::Get()->onRenderPassVfunc1(a_renderPass, args);
}


uintptr_t onRenderPassVfunc2Detour(RE::CreationRendererPrivate::RenderPass* a_renderPass, uintptr_t rdx, uintptr_t rcx, uintptr_t r8, uintptr_t r9) {
    return CreationEngineExperimentModule::Get()->onRenderPassVfunc2(a_renderPass, rdx, rcx, r8, r9);
}

uintptr_t onRenderPassVfunc3Detour(RE::CreationRendererPrivate::RenderPass* a_renderPass, uintptr_t rdx, uintptr_t r8) {
    return CreationEngineExperimentModule::Get()->onRenderPassVfunc3(a_renderPass, rdx, r8);
}

uintptr_t onRenderPassVfunc4Detour(RE::CreationRendererPrivate::RenderPass* a_renderPass, uintptr_t rdx) {
    return CreationEngineExperimentModule::Get()->onRenderPassVfunc4(a_renderPass, rdx);
}

uintptr_t onDllsProduceInputDetour(RE::CreationRendererPrivate::RenderPass* a_renderPass, RE::CreationRendererPrivate::RenderGraphData* rdx, RE::CreationRendererPrivate::RenderPassData* r8) {
    return CreationEngineExperimentModule::Get()->onDllsProduceInput(a_renderPass, rdx, r8);
}

uintptr_t onDllsUpscaleDetour(RE::CreationRendererPrivate::RenderPass* a_renderPass, RE::CreationRendererPrivate::RenderGraphData* rdx, RE::CreationRendererPrivate::RenderPassData* r8) {
    return CreationEngineExperimentModule::Get()->onDllsUpscale(a_renderPass, rdx, r8);
}

uintptr_t onRenderPassVfunc8MotionVectorCompositeDetour(RE::CreationRendererPrivate::RenderPass* a_renderPass, RE::CreationRendererPrivate::RenderGraphData* rdx, RE::CreationRendererPrivate::RenderPassData* r8) {
    return CreationEngineExperimentModule::Get()->onRenderPassVfunc8MotionVectorComposite(a_renderPass, rdx, r8);
}

uintptr_t onSetMotionVectorPassConstantsDetour(uintptr_t pipelineLayout, RE::RenderPassConstantBufferView* camerablocks, RE::MotionVectorRenderPassConstants* motionVectorConstants, uintptr_t renderPassData) {
    return CreationEngineExperimentModule::Get()->onSetMotionVectorPassConstants(pipelineLayout, camerablocks, motionVectorConstants, renderPassData);
}

void onSetSlDlssOptionsDetour(__int64 rcx, __int64 rdx, RE::RenderPassConstantBufferView* pView, __int64 r9, unsigned int a5, bool reset)
{
    return CreationEngineExperimentModule::Get()->onSetSlDlssOptions(rcx, rdx, pView, r9, a5, reset);
}

void onStartRenderPipelineDetour(__int64 rcx, __int64 rdx)
{
    CreationEngineExperimentModule::Get()->onStartRenderPipeline(rcx, rdx);
}

__int64 onVFunc9MainRenderGraphDetour(RE::CreationRendererPrivate::MainRenderGraph* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineExperimentModule::Get()->onVFunc9MainRenderGraph(rcx, rdx, r8, r9);

}

__int64 onVFunc10MainRenderGraphDetour(RE::CreationRendererPrivate::MainRenderGraph* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineExperimentModule::Get()->onVFunc10MainRenderGraph(rcx, rdx, r8, r9);

}

__int64 onVFunc7RenderGraphDetour(RE::CreationRendererPrivate::MainRenderGraph* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineExperimentModule::Get()->onVFunc7RenderGraph(rcx, rdx, r8, r9);
}


__int64 onVFunc8FrameRenderGraphDetour(RE::CreationRendererPrivate::RenderGraph* rcx, __int64 rdx)
{
    return CreationEngineExperimentModule::Get()->onVFunc8FrameRenderGraph(rcx, rdx);
}

__int64 onVFunc9FrameRenderGraphDetour(RE::CreationRendererPrivate::RenderGraph* rcx, __int64 rdx)
{
    return CreationEngineExperimentModule::Get()->onVFunc9FrameRenderGraph(rcx, rdx);
}

__int64 onUnk2MainRenderGraphDetour(void* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineExperimentModule::Get()->onUnk2MainRenderGraph(rcx, rdx, r8, r9);
}

__int64 onUnk3MainRenderGraphDetour(void* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineExperimentModule::Get()->onUnk3MainRenderGraph(rcx, rdx, r8, r9);
}
//
//__int64 onRenderFrameStartDetour(void* rcx, __int64 rdx, __int64 r8, __int64 r9)
//{
//    return CreationEngineExperimentModule::Get()->onRenderFrameStart(rcx, rdx, r8, r9);
//}

//void SetWindowSizeDetour(RE::DisplayGameSettings* pDisplayGameSetting, int width, int height) {
//    return CreationEngineExperimentModule::Get()->SetWindowSize2(pDisplayGameSetting, width, height);
//}

//void onRenderFrameEndDetour(__int64 rcx, __int64 rdx, __int64 r8, __int64 r9)
//{
//    CreationEngineExperimentModule::Get()->onRenderFrameEnd(rcx, rdx, r8, r9);
//}
//
//void onRenderFrameEndDetour2(__int64 rcx, __int64 rdx, __int64 r8, __int64 r9)
//{
//    CreationEngineExperimentModule::Get()->onRenderFrameEnd2(rcx, rdx, r8, r9);
//}
//
//std::unique_ptr<PolyHook2FunctionHook> m_onWindowMessageHook;
//
//LRESULT CALLBACK WndProcDetour(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    using func_t = decltype(WndProcDetour);
//    auto original_func = m_onWindowMessageHook->get_original<func_t>();
////    spdlog::info("Window message hit {}", message);
//    return original_func(hWnd, message, wParam, lParam);
//}
//


void CreationEngineExperimentModule::InstallHooks() {
/*    REL::Relocation<uintptr_t> pipelineGenerationStartedEventVfunc1{ REL::ID(99905) };
    m_onPipelineGenerationStartedEventHook = std::make_unique<PolyHook2FunctionHook>(pipelineGenerationStartedEventVfunc1.address(), reinterpret_cast<uintptr_t>(&onPipelineGenerationStartedEventDetour));
    m_onPipelineGenerationStartedEventHook->create();

    REL::Relocation<uintptr_t> pipelineGenerationStartedEventVfunc2{ REL::ID(99907) };
    m_onPipelineGenerationStartedEventHook2 = std::make_unique<PolyHook2FunctionHook>(pipelineGenerationStartedEventVfunc2.address(), reinterpret_cast<uintptr_t>(&onPipelineGenerationStartedEvent2Detour));
    m_onPipelineGenerationStartedEventHook2->create();

    REL::Relocation<uintptr_t> pipelineGenerationStartedEventLambdaAddr{ REL::ID(148822) };
    m_onPipelineGenerationStartedEventLambdaHook = std::make_unique<PolyHook2FunctionHook>(pipelineGenerationStartedEventLambdaAddr.address(), reinterpret_cast<uintptr_t>(&onPipelineGenerationStartedEventLambdaDetour));
    m_onPipelineGenerationStartedEventLambdaHook->create();

    REL::Relocation<uintptr_t> renderPassVfunc1Addr{ REL::ID(206083) };
    m_onVfunc1RenderPassHook = std::make_unique<PolyHook2FunctionHook>(renderPassVfunc1Addr.address(), reinterpret_cast<uintptr_t>(&onRenderPassVfunc1Detour));
    m_onVfunc1RenderPassHook->create();*/

//    REL::Relocation<uintptr_t> setSlDlssOptionsFuncAddr{ REL::ID(1078914) };
//    m_onSetDlssOptions = std::make_unique<PolyHook2FunctionHook>(setSlDlssOptionsFuncAddr.address(), reinterpret_cast<uint64_t>(&onSetSlDlssOptionsDetour));
//    m_onSetDlssOptions->create();
//
//    REL::Relocation<uintptr_t> startRenderPipelineAddr{ REL::ID(1079055) };
//    m_OnStartRenderPipelineHook = std::make_unique<PolyHook2FunctionHook>(startRenderPipelineAddr.address(), reinterpret_cast<uint64_t>(&onStartRenderPipelineDetour));
//    m_OnStartRenderPipelineHook->create();
/*    REL::Relocation<uintptr_t> vfuncMRG{ REL::ID(208565) };
    m_onVfunc9RenderGraphHook = std::make_unique<PolyHook2FunctionHook>(vfuncMRG.address(), reinterpret_cast<uint64_t>(&onVFunc9MainRenderGraphDetour));
    m_onVfunc9RenderGraphHook->create();

    REL::Relocation<uintptr_t> vfuncMRG10{ REL::ID(208565) };
    m_onVfunc10RenderGraphHook = std::make_unique<PolyHook2FunctionHook>(vfuncMRG10.address(), reinterpret_cast<uint64_t>(&onVFunc10MainRenderGraphDetour));
    m_onVfunc10RenderGraphHook->create();


    REL::Relocation<uintptr_t> frameRenderGraphVfunc8{ REL::ID(208566) };
    m_onVfunc8FrameRenderGraphHook = std::make_unique<PolyHook2FunctionHook>(frameRenderGraphVfunc8.address(), reinterpret_cast<uint64_t>(&onVFunc8FrameRenderGraphDetour));
    m_onVfunc8FrameRenderGraphHook->create();

    REL::Relocation<uintptr_t> frameRenderGraphVfunc9{ REL::ID(208567) };
    m_onVfunc9FrameRenderGraphHook = std::make_unique<PolyHook2FunctionHook>(frameRenderGraphVfunc9.address(), reinterpret_cast<uint64_t>(&onVFunc9FrameRenderGraphDetour));
    m_onVfunc9FrameRenderGraphHook->create();*/

//    REL::Relocation<uintptr_t> renderPassVfunc1Addr{ REL::ID(206083) };
//    m_onVfunc1RenderPassHook = std::make_unique<PolyHook2FunctionHook>(renderPassVfunc1Addr.address(), reinterpret_cast<uintptr_t>(&onRenderPassVfunc1Detour));
//    m_onVfunc1RenderPassHook->create();

//    REL::Relocation<uintptr_t> vFunc2RenderPassAddr{ REL::ID(206087) };
//    m_onVfunc2RenderPassHook = std::make_unique<PolyHook2FunctionHook>(vFunc2RenderPassAddr.address(), reinterpret_cast<uint64_t>(&onRenderPassVfunc2Detour));
//    m_onVfunc2RenderPassHook->create();
//
//    REL::Relocation<uintptr_t> vFunc3RenderGraphAddr{ REL::ID(206082) };
//    m_onVfunc3RenderPassHook = std::make_unique<PolyHook2FunctionHook>(vFunc3RenderGraphAddr.address(), reinterpret_cast<uint64_t>(&onRenderPassVfunc3Detour));
//    m_onVfunc3RenderPassHook->create();
//
//    REL::Relocation<uintptr_t> vFunc4RenderGraphAddr{ REL::ID(206084) };
//    m_onVfunc4RenderPassHook = std::make_unique<PolyHook2FunctionHook>(vFunc4RenderGraphAddr.address(), reinterpret_cast<uint64_t>(&onRenderPassVfunc4Detour));
//    m_onVfunc4RenderPassHook->create();

//    REL::Relocation<uintptr_t> vFunc7RenderGraphAddr{ REL::ID(1079046) };
//    m_onVfunc7RenderGraphHook = std::make_unique<PolyHook2FunctionHook>(vFunc7RenderGraphAddr.address(), reinterpret_cast<uint64_t>(&onVFunc7RenderGraphDetour));
//    m_onVfunc7RenderGraphHook->create();

//    REL::Relocation<uintptr_t> onDllsProduceInputVfunc8Addr{ REL::ID(1079188) };
//    m_onVfunc8RenderPassDlssProduceInputsHook = std::make_unique<PolyHook2FunctionHook>(onDllsProduceInputVfunc8Addr.address(), reinterpret_cast<uint64_t>(&onDllsProduceInputDetour));
//    m_onVfunc8RenderPassDlssProduceInputsHook->create();

//    REL::Relocation<uintptr_t> onDllsUpscaleVfunc8Addr{ REL::ID(1079131) };
//    m_onVfunc8RenderPassDlssUpscaleHook = std::make_unique<PolyHook2FunctionHook>(onDllsUpscaleVfunc8Addr.address(), reinterpret_cast<uint64_t>(&onDllsUpscaleDetour));
//    m_onVfunc8RenderPassDlssUpscaleHook->create();

//    REL::Relocation<uintptr_t> onRenderGraphRenonMotionVectorCompositeVfunct8Addr{ REL::ID(208028) };
//    m_onVfunc8RenderPassMotionVectorCompositeHook = std::make_unique<PolyHook2FunctionHook>(onRenderGraphRenonMotionVectorCompositeVfunct8Addr.address(), reinterpret_cast<uint64_t>(&onRenderPassVfunc8MotionVectorCompositeDetour));
//    m_onVfunc8RenderPassMotionVectorCompositeHook->create();


//    REL::Relocation<uintptr_t> onSetMotionVectorPassConstants{ REL::ID(1275380) };
//    m_onSetMotionVectorPassConstantsHook = std::make_unique<PolyHook2FunctionHook>(onSetMotionVectorPassConstants.address(), reinterpret_cast<uint64_t>(&onSetMotionVectorPassConstantsDetour));
//    m_onSetMotionVectorPassConstantsHook->create();

    // this function is rendering frame start to end except presenting(probably)
    // any heavy weight operation we need to avoid
//    REL::Relocation<uintptr_t> onRenderGraphRenderStartFuncAddr{ REL::ID(1079045) };
//    REL::Relocation<uintptr_t> onRenderGraphRenderStartFuncAddr{ RE2::MemoryOffsets::CreationRenderer::RenderGraphFrameStart() };
//    m_onRenderGraphRenderStartHook
//        = std::make_unique<PolyHook2FunctionHook>(onRenderGraphRenderStartFuncAddr.address(), reinterpret_cast<uint64_t>(&onRenderGraphRenderStartDetour));
//    m_onRenderGraphRenderStartHook->create();

//    REL::Relocation<uintptr_t> mainRenderGraphUnk2Func{ REL::ID(1078894) };
//    m_onUnk2RenderGraphHook = std::make_unique<PolyHook2FunctionHook>(mainRenderGraphUnk2Func.address(), reinterpret_cast<uint64_t>(&onUnk2MainRenderGraphDetour));
//    m_onUnk2RenderGraphHook->create();

//    REL::Relocation<uintptr_t> mainRenderGraphUnk3Func{ REL::ID(202136) };
//    m_onUnk3RenderGraphHook = std::make_unique<PolyHook2FunctionHook>(mainRenderGraphUnk3Func.address(), reinterpret_cast<uint64_t>(&onUnk3MainRenderGraphDetour));
//    m_onUnk3RenderGraphHook->create();

    // ID is exactly incrementing frames 149000
    // 202136 ID is function on start is frame start on end is frame end, however it is significantly decreases fps
//    REL::Relocation<uintptr_t> onRenderFrameStartFuncAddr{ REL::ID(202136) };
//    REL::Relocation<uintptr_t> onRenderFrameStartFuncAddr{ RE2::MemoryOffsets::CreationRenderer::RenderGraphRenderPipelineExecute() };
//    m_onRenderFrameStartHook = std::make_unique<PolyHook2FunctionHook>(onRenderFrameStartFuncAddr.address(), reinterpret_cast<uint64_t>(&onRenderFrameStartDetour));
//    m_onRenderFrameStartHook->create();
//
////    REL::Relocation<RE::CreationEngineSettings**> settings{ REL::ID(878340) };
//    REL::Relocation<RE::CreationEngineSettings**> settings{ RE2::MemoryOffsets::GlobalRenderSettings() };
//    m_creationEngineSettings = settings.get();
//
//    REL::Relocation<int*> globalFrameCountAddr{ REL::ID(885900)};
//    m_globalFrameCount = globalFrameCountAddr.get();

//    static REL::Relocation<uintptr_t> funcSetWindowSize{REL::ID{1275258}};
//    m_onSetWindoSizeHook = std::make_unique<PolyHook2FunctionHook>(funcSetWindowSize.address(), reinterpret_cast<uintptr_t >(&SetWindowSizeDetour));
//    m_onSetWindoSizeHook->create();

    //    REL::Relocation<uintptr_t> onWindowMessageAddr{ REL::ID(149060) };
//    m_onWindowMessageHook = std::make_unique<PolyHook2FunctionHook>(onWindowMessageAddr.address(), reinterpret_cast<uint64_t>(&WndProcDetour));
//    m_onWindowMessageHook->create();

//    REL::Relocation<uintptr_t> onRenderFrameEndFuncAddr{ REL::ID(148802) };
//    m_onRenderFrameEndHook = std::make_unique<PolyHook2FunctionHook>(onRenderFrameEndFuncAddr.address(), reinterpret_cast<uint64_t>(&onRenderFrameEndDetour));
//    m_onRenderFrameEndHook->create();
//    REL::Relocation<uintptr_t> onRenderFrameEndFuncAddr2{ REL::ID(149000) };
//    m_onRenderFrameEndHook2 = std::make_unique<PolyHook2FunctionHook>(onRenderFrameEndFuncAddr2.address(), reinterpret_cast<uint64_t>(&onRenderFrameEndDetour2));
//    m_onRenderFrameEndHook2->create();
}


uintptr_t CreationEngineExperimentModule::onPipelineGenerationStartedEvent(uintptr_t i, uintptr_t i1)
{
    using func_t = decltype(onPipelineGenerationStartedEventDetour);
    auto original_func = m_onPipelineGenerationStartedEventHook->get_original<func_t>();
    spdlog::info("Pipeline generation started event vfunc 1 {:x}", i);
    return original_func(i, i1);
}

uintptr_t CreationEngineExperimentModule::onPipelineGenerationStartedEvent2(uintptr_t i)
{
    using func_t = decltype(onPipelineGenerationStartedEvent2Detour);
    auto original_func = m_onPipelineGenerationStartedEventHook2->get_original<func_t>();
    spdlog::info("Pipeline generation started event vfunc 2 {:x}", i);
    return original_func(i);
}

uintptr_t CreationEngineExperimentModule::onPipelineGenerationStartedEventLambda(uintptr_t i)
{
    spdlog::info("Pipeline generation started event lambda {:x}", i);
    using func_t = decltype(onPipelineGenerationStartedEventLambdaDetour);
    auto original_func = m_onPipelineGenerationStartedEventLambdaHook->get_original<func_t>();
    return original_func(i);
}

uintptr_t CreationEngineExperimentModule::onRenderPassVfunc1(RE::CreationRendererPrivate::RenderPass* pass, va_list args)
{
    using func_t = decltype(onRenderPassVfunc1Detour);
    auto original_func = m_onVfunc1RenderPassHook->get_original<func_t>();
    spdlog::info("Render pass vfunc 1 {} {}", fmt::ptr(pass), pass->name);
    return original_func(pass, args);
}

#define BYTEn(x, n) (*((uint8_t*)&(x)+n))

uintptr_t CreationEngineExperimentModule::onRenderPassVfunc2(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i, uintptr_t i1, uintptr_t i2, uintptr_t i3)
{
    using func_t = decltype(onRenderPassVfunc2Detour);
    auto original_func = m_onVfunc2RenderPassHook->get_original<func_t>();
    auto enabled = BYTEn(pass->flags, 2);
    spdlog::info("Render pass vfunc 2 {} {} enabled={}", fmt::ptr(pass), pass->name, enabled);
    return original_func(pass, i, i1, i2, i3);
}


uintptr_t CreationEngineExperimentModule::onRenderPassVfunc3(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i, uintptr_t i1)
{
    using func_t = decltype(onRenderPassVfunc3Detour);
    auto original_func = m_onVfunc3RenderPassHook->get_original<func_t>();
    // check if 2-ths bit of flags == 1
    auto enabled = BYTEn(pass->flags, 2);
    if(enabled == 1) {
        spdlog::info("Render pass vfunc 3 {} {} enabled={}", fmt::ptr(pass), pass->name, enabled);
    }
    return original_func(pass, i, i1);
}

uintptr_t CreationEngineExperimentModule::onRenderPassVfunc4(RE::CreationRendererPrivate::RenderPass* pass, uintptr_t i)
{
    using func_t = decltype(onRenderPassVfunc4Detour);
    auto original_func = m_onVfunc4RenderPassHook->get_original<func_t>();
    // check if 2-ths bit of flags == 1
    auto enabled = BYTEn(pass->flags, 2);
    if(enabled == 1) {
        spdlog::info("Render pass vfunc 4 {} {} enabled={}", fmt::ptr(pass), pass->name, enabled);
    }
    return original_func(pass, i);
}

void CreationEngineExperimentModule::onSetSlDlssOptions(__int64 i, __int64 i1, RE::RenderPassConstantBufferView* pView, __int64 i3, unsigned int i4, bool b) {
    using func_t = decltype(onSetSlDlssOptionsDetour);
    auto original_func = m_onSetDlssOptions->get_original<func_t>();
//    spdlog::info("Set DLSS options hit {}", fmt::ptr(pView));
    auto vr = VR::get();
    if(vr->m_frame_count%2 == 0) {
        g_dlss_debug_info.currentProjectionMatrix = pView->unk50;
        g_dlss_debug_info.currentCameraPosition = {pView->unk0.x, pView->unk0.y, 0.0f, (float)pView->unk8};
        g_dlss_debug_info.previousProjectionMatrix = pView->unk150;
        g_dlss_debug_info.previousCameraPosition = {pView->unk190.x, pView->unk190.y, 0.0f, (float)pView->unk198};
    }
    return original_func(i, i1, pView, i3, i4, b);
}

void CreationEngineExperimentModule::onStartRenderPipeline(__int64 i, __int64 i1) {
    using func_t = decltype(onStartRenderPipelineDetour);
    auto original_func = m_OnStartRenderPipelineHook->get_original<func_t>();
    spdlog::info("Not A Start render pipeline hit");
    return original_func(i, i1);
}

__int64 CreationEngineExperimentModule::onVFunc9MainRenderGraph(RE::CreationRendererPrivate::MainRenderGraph* graph, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onVFunc9MainRenderGraphDetour);
    auto original_func = m_onVfunc9RenderGraphHook->get_original<func_t>();
//    spdlog::info("Main render graph vfunc 9 {} {}", fmt::ptr(graph), graph->name);
    return original_func(graph, i, i1, i2);
}

__int64 CreationEngineExperimentModule::onVFunc8FrameRenderGraph(RE::CreationRendererPrivate::RenderGraph* pGraph, __int64 i)
{
    using func_t = decltype(onVFunc8FrameRenderGraphDetour);
    auto original_func = m_onVfunc8FrameRenderGraphHook->get_original<func_t>();
//    spdlog::info("Frame render graph vfunc 8 {} {}", fmt::ptr(pGraph), pGraph->name);
    return original_func(pGraph, i);
}

__int64 CreationEngineExperimentModule::onVFunc9FrameRenderGraph(RE::CreationRendererPrivate::RenderGraph* pGraph, __int64 i)
{
    using func_t = decltype(onVFunc9FrameRenderGraphDetour);
    auto original_func = m_onVfunc9FrameRenderGraphHook->get_original<func_t>();
//    spdlog::info("Frame render graph vfunc 9 {} {}", fmt::ptr(pGraph), pGraph->name);
    return original_func(pGraph, i);
}

__int64 CreationEngineExperimentModule::onVFunc10MainRenderGraph(RE::CreationRendererPrivate::MainRenderGraph* pGraph, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onVFunc10MainRenderGraphDetour);
    auto original_func = m_onVfunc10RenderGraphHook->get_original<func_t>();
//    spdlog::info("Main render graph vfunc 10 {} {}", fmt::ptr(pGraph), pGraph->name);
    return original_func(pGraph, i, i1, i2);
}

/*
void CreationEngineExperimentModule::RestoreBuffer(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* originalBuffer, int index, int slot) {
    if(!ValidateResource(originalBuffer, m_pastBuffer[index])) {
        return;
    }
    auto saveBuffer = m_pastBuffer[index][slot].Get();
    CopyResource(cmdList, saveBuffer, originalBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

bool CreationEngineExperimentModule::SaveBuffer(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* originalBuffer, int index, int slot) {
    if(!ValidateResource(originalBuffer, m_pastBuffer[index])) {
        return;
    }
    auto saveBuffer = m_pastBuffer[index][slot].Get();
    CopyResource(cmdList, saveBuffer, originalBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}*/

__int64 CreationEngineExperimentModule::onUnk2MainRenderGraph(void* pGraph, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onUnk2MainRenderGraphDetour);
    auto original_func = m_onUnk2RenderGraphHook->get_original<func_t>();
//    spdlog::info("Upper MRG possible ");
    return original_func(pGraph, i, i1, i2);
}

__int64 CreationEngineExperimentModule::onUnk3MainRenderGraph(void* pVoid, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onUnk3MainRenderGraphDetour);
    auto original_func = m_onUnk3RenderGraphHook->get_original<func_t>();
    auto result = original_func(pVoid, i, i1, i2);
    return result;
}

__int64 CreationEngineExperimentModule::onVFunc7RenderGraph(RE::CreationRendererPrivate::MainRenderGraph* pGraph, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onVFunc7RenderGraphDetour);
    auto original_func = m_onVfunc7RenderGraphHook->get_original<func_t>();
//    if(pGraph->renderGraphID == RE::Main::WorldRoot()->renderHandleID) {
//        RE::MultiCameraViewModule::ResetUpscalerHistoryData();
//    }
    return original_func(pGraph, i, i1, i2);
}

uintptr_t CreationEngineExperimentModule::onDllsProduceInput(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1)
{
    using func_t = decltype(onDllsProduceInputDetour);
    auto original_func = m_onVfunc8RenderPassDlssProduceInputsHook->get_original<func_t>();
    spdlog::info("DLSS produce inputs depth={} state={} input={} state={}",  i1->getRenderPassItemByIndex(1)->getDXGIState(), fmt::ptr(i1->getNativeResourceByIndex(1)), fmt::ptr(i1->getNativeResourceByIndex(0)), i1->getRenderPassItemByIndex(0)->getDXGIState());
    return original_func(pPass, i, i1);
}

uintptr_t CreationEngineExperimentModule::onDllsUpscale(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1)
{
    using func_t = decltype(onDllsUpscaleDetour);
    auto original_func = m_onVfunc8RenderPassDlssUpscaleHook->get_original<func_t>();
//    spdlog::info("onDllsUpscale motion={} state={} depth={} state={}",  i1->getRenderPassItemByIndex(1)->getDXGIState(), fmt::ptr(i1->getNativeResourceByIndex(1)), fmt::ptr(i1->getNativeResourceByIndex(3)), i1->getRenderPassItemByIndex(3)->getDXGIState());
    auto dlssModule = NvidiaInterposterExperiment::Get();
    if(dlssModule->isD3D12Inited()) {
        auto context = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(i->getCommandList());
        auto commandList = context->pID3D12CommandList;
        auto depthSource  = i1->getNativeResourceByIndex(3);
        auto depthPassIte = i1->getRenderPassItemByIndex(3);
        auto depthState = static_cast<D3D12_RESOURCE_STATES>(depthPassIte->getDXGIState());
        auto depthDest = dlssModule->GetDepthResourceCopy();
        auto mvecSource = i1->getNativeResourceByIndex(1);
        auto mvecPassItem = i1->getRenderPassItemByIndex(1);
        auto mvecState = static_cast<D3D12_RESOURCE_STATES>(mvecPassItem->getDXGIState());
        auto mvecDest = dlssModule->GetMvecResourceCopy();
        D3D12_RESOURCE_BARRIER barriers[4]{ CD3DX12_RESOURCE_BARRIER::Transition(depthSource, depthState, D3D12_RESOURCE_STATE_COPY_SOURCE),
                                            CD3DX12_RESOURCE_BARRIER::Transition(mvecSource, mvecState, D3D12_RESOURCE_STATE_COPY_SOURCE),
                                            CD3DX12_RESOURCE_BARRIER::Transition(depthDest, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                                                 D3D12_RESOURCE_STATE_COPY_DEST),
                                            CD3DX12_RESOURCE_BARRIER::Transition(mvecDest, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                                                 D3D12_RESOURCE_STATE_COPY_DEST)
        };
        commandList->ResourceBarrier(4, barriers);

        commandList->CopyResource(depthDest, depthSource);
//        commandList->CopyResource(mvecDest, mvecSource);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[0].Transition.StateAfter  = depthState;
        barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[1].Transition.StateAfter  = mvecState;
        barriers[2].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[2].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barriers[3].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[3].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(4, barriers);
    }
    return original_func(pPass, i, i1);
}

uintptr_t CreationEngineExperimentModule::onRenderPassVfunc8MotionVectorComposite(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* i, RE::CreationRendererPrivate::RenderPassData* i1)
{
    using func_t = decltype(onRenderPassVfunc8MotionVectorCompositeDetour);
    auto original_func = m_onVfunc8RenderPassMotionVectorCompositeHook->get_original<func_t>();
//    spdlog::info("MotionVectorsComposite motion={} state={} input={} state={}",  i1->getRenderPassItemByIndex(1)->getDXGIState(), fmt::ptr(i1->getNativeResourceByIndex(1)), fmt::ptr(i1->getNativeResourceByIndex(0)), i1->getRenderPassItemByIndex(0)->getDXGIState());
    auto result = original_func(pPass, i, i1);
    auto dlssModule = NvidiaInterposterExperiment::Get();
    if(dlssModule->isD3D12Inited() && g_dlss_settings.enabled) {
        auto context = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(i->getCommandList());
        auto commandList = context->pID3D12CommandList;
        auto depthDest = dlssModule->GetDepthResourceCopy();
        auto mvecSource = i1->getNativeResourceByIndex(1);
        auto mvecPassItem = i1->getRenderPassItemByIndex(1);
        auto mvecState = static_cast<D3D12_RESOURCE_STATES>(mvecPassItem->getDXGIState());
        auto mvecDest = dlssModule->GetMvecResourceCopy();
        D3D12_RESOURCE_BARRIER barriers[2]{CD3DX12_RESOURCE_BARRIER::Transition(depthDest, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                                                 D3D12_RESOURCE_STATE_COPY_DEST),
                                            CD3DX12_RESOURCE_BARRIER::Transition(mvecDest, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                                                 D3D12_RESOURCE_STATE_COPY_DEST)
        };
        commandList->ResourceBarrier(2, barriers);

        commandList->CopyResource(mvecDest, mvecSource);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[0].Transition.StateAfter  = mvecState;
        barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(2, barriers);
    }
    return result;
}

uintptr_t CreationEngineExperimentModule::onSetMotionVectorPassConstants(uintptr_t i, RE::RenderPassConstantBufferView* pView, RE::MotionVectorRenderPassConstants* constants, uintptr_t renderPassData)
{
    using func_t = decltype(onSetMotionVectorPassConstantsDetour);
    auto original_func = m_onSetMotionVectorPassConstantsHook->get_original<func_t>();

    spdlog::info("Set motion vector pass constants hit {} {} render pass data {:x}", fmt::ptr(pView), fmt::ptr(constants), renderPassData);
//    spdlog::info("distance {}", glm::distance(constants->currentCameraPosition, constants->previousCameraPosition));
    return original_func(i, pView, constants, renderPassData);
}

//void CreationEngineExperimentModule::onRenderFrameEnd(__int64 i, __int64 i1, __int64 i2, __int64 i3) {
//    using func_t = decltype(onRenderFrameEndDetour);
//    auto original_func = m_onRenderFrameEndHook->get_original<func_t>();
//    spdlog::info("Lod Adjust incrementing frame count");
////    auto vr = VR::get();
////    vr->on_post_end_rendering(nullptr);
//    auto vr = VR::get();
//    vr->on_pre_begin_rendering(nullptr);
//    vr->on_begin_rendering(nullptr);
//
//    original_func(i, i1, i2, i3);
//}
//
//void CreationEngineExperimentModule::onRenderFrameEnd2(__int64 i, __int64 i1, __int64 i2, __int64 i3) {
//    using func_t = decltype(onRenderFrameEndDetour2);
//    auto original_func = m_onRenderFrameEndHook2->get_original<func_t>();
//    spdlog::info("Another  incrementing frame count");
//    auto vr = VR::get();
//    vr->on_pre_begin_rendering(nullptr);
//    vr->on_begin_rendering(nullptr);
//    original_func(i, i1, i2, i3);
//}
#endif