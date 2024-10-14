//
// Created by sergp on 6/26/2024.
//

#include "CreationEngineRendererModule.h"
#include "CreationEngineConstants.h"
#include "CreationEngineSingletonManager.h"
#include "REL/Relocation.h"
#include "UpscalerAfrNvidiaModule.h"
#include <CreationEngine/memory/offsets.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>
#include <mods/VR.hpp>

uintptr_t onUpdateConstantBufferViewDetour(uint8_t copyPresentToPast, uint8_t orphoProjection, uintptr_t pCameraTransforms, uintptr_t vararg1Parent, uintptr_t vararg3Parent, double unk, char unk2, RE::RenderPassConstantBufferView* camerablocks) {
    return CreationEngineRendererModule::Get()->onUpdateConstantBufferView(copyPresentToPast, orphoProjection, pCameraTransforms, vararg1Parent, vararg3Parent, unk, unk2, camerablocks);
}

__int64 onRenderGraphRenderStartDetour(RE::CreationRendererPrivate::RenderGraph* rcx, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, __int64 r8, __int64 r9)
{
    return CreationEngineRendererModule::Get()->onRenderGraphRenderStart(rcx, pRenderGraphData, r8, r9);
}

__int64 onRenderFrameStartDetour(void* rcx, __int64 rdx, __int64 r8, __int64 r9)
{
    return CreationEngineRendererModule::Get()->onRenderFrameStart(rcx, rdx, r8, r9);
}

std::unique_ptr<FunctionHook> m_onWindowMessageHook;

LRESULT CALLBACK WndProcDetour(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    using func_t = decltype(WndProcDetour);
    static auto original_func = m_onWindowMessageHook->get_original<func_t>();
//    spdlog::info("Window message hit {}", message);
    return original_func(hWnd, message, wParam, lParam);
}



void CreationEngineRendererModule::InstallHooks() {

    //    REL::Relocation<RE::CreationEngineSettings**> settings{ REL::ID(878340) };
    REL::Relocation<RE::CreationEngineSettings**> settings{ GameStore::MemoryOffsets::GlobalRenderSettings() };
    m_creationEngineSettings = settings.get();

    REL::Relocation<int*> globalFrameCountAddr{ GameStore::MemoryOffsets::CreationRenderer::GlobalFrameCount() };
    m_globalFrameCount = globalFrameCountAddr.get();


    REL::Relocation<uintptr_t> onUpdateConstantBufferViewAddr{ GameStore::MemoryOffsets::CreationRenderer::OnUpdateConstantBufferView() };
    m_onUpdateConstantBufferViewHook = std::make_unique<FunctionHook>(onUpdateConstantBufferViewAddr.address(), reinterpret_cast<uint64_t>(&onUpdateConstantBufferViewDetour));
    m_onUpdateConstantBufferViewHook->create();

//    REL::Relocation<uintptr_t> onRenderGraphRenderStartFuncAddr{ REL::ID(1079045) };
    REL::Relocation<uintptr_t> onRenderGraphRenderStartFuncAddr{ GameStore::MemoryOffsets::CreationRenderer::RenderGraphFrameStart() };
    m_onRenderGraphRenderStartHook = std::make_unique<FunctionHook>(onRenderGraphRenderStartFuncAddr.address(), reinterpret_cast<uint64_t>(&onRenderGraphRenderStartDetour));
    m_onRenderGraphRenderStartHook->create();

    // ID is exactly incrementing frames 149000
    // 202136 ID is function on start is frame start on end is frame end, however it is significantly decreases fps
//    REL::Relocation<uintptr_t> onRenderFrameStartFuncAddr{ REL::ID(202136) };
    REL::Relocation<uintptr_t> onRenderFrameStartFuncAddr{ GameStore::MemoryOffsets::CreationRenderer::RenderGraphRenderPipelineExecute() };
    m_onRenderFrameStartHook = std::make_unique<FunctionHook>(onRenderFrameStartFuncAddr.address(), reinterpret_cast<uint64_t>(&onRenderFrameStartDetour));
    m_onRenderFrameStartHook->create();

    REL::Relocation<uintptr_t> taa_vfunc7_hook_addr{ GameStore::MemoryOffsets::CreationRenderer::OnTaaVFunc7() };
    taa_vfunc7_hook = std::make_unique<FunctionHook>(taa_vfunc7_hook_addr.address(), reinterpret_cast<uint64_t>(&CreationEngineRendererModule::onTaaPass));
    taa_vfunc7_hook->create();
}

struct CameraBlockSnapshot {
    Matrix4x4f cameraMatrix;
    Vector2f cameraPositionOrJitter;
    int frameNumber;
};
std::unordered_map<uintptr_t,CameraBlockSnapshot> pastProjections{};

__int64 CreationEngineRendererModule::onRenderGraphRenderStart(RE::CreationRendererPrivate::RenderGraph* pGraph, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, __int64 i1, __int64 i2)
{
    using func_t = decltype(onRenderGraphRenderStartDetour);
    static auto original_func = m_onRenderGraphRenderStartHook->get_original<func_t>();
    RenderGraphStart(pGraph, pRenderGraphData, true);
    auto result = original_func(pGraph, pRenderGraphData, i1, i2);
    RenderGraphStart(pGraph, pRenderGraphData, false);
    return result;
}

bool CreationEngineRendererModule::ValidateResource(ID3D12Resource* source, ComPtr<ID3D12Resource> pastBuffer[4])
{
    if(source == nullptr) {
        return false;
    }
    D3D12_RESOURCE_DESC desc = source->GetDesc();
    if(pastBuffer[0] != nullptr) {
        D3D12_RESOURCE_DESC desc2 = pastBuffer[0]->GetDesc();
        if(desc.Width != desc2.Width || desc.Height != desc2.Height || desc.Format != desc2.Format) {
            spdlog::info("Resource size mismatch {} {} {} {} {} {} {} {}", fmt::ptr(source), desc.Width, desc.Height, desc.Format, fmt::ptr(pastBuffer[0].Get()), desc2.Width, desc2.Height, desc2.Format);
            pastBuffer[0].Reset();
            pastBuffer[1].Reset();
            pastBuffer[2].Reset();
            pastBuffer[3].Reset();
        }
    }
    auto device = g_framework->get_d3d12_hook()->get_device();
    if(device == nullptr) {
        return false;
    }
    if(pastBuffer[0] == nullptr || pastBuffer[1] == nullptr) {
        D3D12_HEAP_PROPERTIES heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        for(int i=0; i<4; i++) {
            if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                       IID_PPV_ARGS(pastBuffer[i].GetAddressOf()))))
            {
                spdlog::error("[VR] Failed to create resource copy copy for AFR backbuffer {}.", i);
                return false;
            }
        }
    }
    return true;
}

void CreationEngineRendererModule::SwapBuffer(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* originalBuffer, int index, int copyFromSlot, int copyToSlot) {
    if(!ValidateResource(originalBuffer, m_pastBuffer[index])) {
        return;
    }
    auto saveBuffer = m_pastBuffer[index][copyToSlot].Get();
    auto alterFrameBuffer = m_pastBuffer[index][copyFromSlot].Get();
    CopyResource(cmdList,originalBuffer , saveBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
    CopyResource(cmdList, alterFrameBuffer, originalBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void CreationEngineRendererModule::RenderGraphStart(RE::CreationRendererPrivate::RenderGraph* pGraph, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, bool before)
{
    //    RE::StorageTable::RenderGraphHandle::ResetUpscalerHistory(pGraph->renderGraphID);
    if (m_startFramePass == nullptr && strcmp(pGraph->name, "CRBeginFrame") == 0) {
        m_startFramePass = pGraph;
    }
    if (m_endFramePass == nullptr && strcmp(pGraph->name, "CREndFrame") == 0) {
        m_endFramePass = pGraph;
    }
    if (m_framePass == nullptr && strcmp(pGraph->name, "Frame") == 0) {
        m_framePass = pGraph;
    }
    auto vr = VR::get();
    if (m_startFramePass == pGraph) {
        if (before) {
            vr->on_pre_begin_rendering(nullptr);
        }
        else {
            vr->on_begin_rendering(nullptr);
        }
    }
    else if (m_endFramePass == pGraph) {
        if (before) {
            vr->on_pre_end_rendering(nullptr);
        }
        else {
            vr->on_end_rendering(nullptr);
        }
    }
    else if (!before && m_framePass == pGraph) {
        auto fc       = m_globalFrameCount == nullptr? 0: *m_globalFrameCount;
        auto context     = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(pRenderGraphData->getCommandList());
        auto commandList = context->pID3D12CommandList;
        if(vr->is_hmd_active() && Constants::enabledResourcesToCopy[1]) {
            auto resource = pRenderGraphData->getResourceByIndex(1, (fc - 1) & 1);
            SwapBuffer(commandList, resource, 1,  (fc - 1) & 1, fc & 1);
        }
    }
}

__int64 CreationEngineRendererModule::onRenderFrameStart(void* pVoid, __int64 i, __int64 i1, __int64 i2)
{
    using func_t = decltype(onRenderFrameStartDetour);
    static auto original_func = m_onRenderFrameStartHook->get_original<func_t>();
    auto vr = VR::get();
    if(vr->get_runtime()->ready() || Constants::cameraShake) {
        vr->m_frame_count++;
    }
    g_framework->run_imgui_frame(false);

//    spdlog::info("Frame count increase {}", vr->m_frame_count);
    auto result = original_func(pVoid, i, i1, i2);
    return result;
}

void CreationEngineRendererModule::SetWindowSize(int width, int height) {
    {
        std::lock_guard _{m_last_resolution_mutex};
        if(*m_creationEngineSettings == nullptr || (std::chrono::steady_clock::now() - m_last_resolution_sync) < std::chrono::seconds(5)) {
            return;
        }
        m_last_resolution_sync = std::chrono::steady_clock::now();
    }
    if(width == 0 || height == 0) {
        auto vr = VR::get();
        width = vr->get_hmd_width();
        height = vr->get_hmd_height();
    }
    auto window = (*m_creationEngineSettings)->pHwindow;
    int windowWidth = window->windowCX - window->windowX;
    int windowHeight = window->windowCY - window->windowY;
    if(windowWidth == width && windowHeight == height) {
        return;
    }

    spdlog::info("Setting window size to {} {}", width, height);
    auto rect = &(*m_creationEngineSettings)->displayGameSettings.displayRect;

    rect->cx = width+rect->x;
    rect->cy = height+rect->y;
    (*m_creationEngineSettings)->displayGameSettings.flags |= 0x100;
    WINDOWPOS pos;
    pos.hwnd = (*m_creationEngineSettings)->pHwindow->windowHandle;
    pos.hwndInsertAfter = nullptr;
    pos.x = rect->x;
    pos.y = rect->y;
    pos.cx = width;
    pos.cy = height;
    pos.flags = 0;
    SendMessage((*m_creationEngineSettings)->pHwindow->windowHandle, WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM>(&pos));
}

uintptr_t CreationEngineRendererModule::onUpdateConstantBufferView(uint8_t copyCurrentToPast, uint8_t resetHistory, uintptr_t i2, uintptr_t i3, uintptr_t i4, double d, char i5,
                                                                   RE::RenderPassConstantBufferView* pView)
{
    using func_t = decltype(onUpdateConstantBufferViewDetour);
    static auto original_func = m_onUpdateConstantBufferViewHook->get_original<func_t>();
    auto vr = VR::get();
    auto result =  original_func(copyCurrentToPast, resetHistory, i2, i3, i4, d, i5, pView);
//    spdlog::info("update camera blocks {} index {} handles[{},{},{}] st ptr {}", fmt::ptr(pView), indexAfterPresent, cameraHandleId, cameraHandleId2, cameraHandleId3, fmt::ptr(cameraBlocks->data));
    if(copyCurrentToPast && vr->is_hmd_active() && Constants::enableAFRCameraMotionFix) {
        auto key = reinterpret_cast<uintptr_t>(pView) & 0xFFFFFFFFFFFFFFC0;
        if(pastProjections.find(key) != pastProjections.end()) {
            auto temp = pastProjections[key];
            auto past2past = &pastProjections[key];
            past2past->cameraMatrix = pView->unk150;
            past2past->cameraPositionOrJitter = pView->unk190;
            past2past->frameNumber = pView->unk198;
            if(!resetHistory) {
                pView->unk150 = temp.cameraMatrix;
                pView->unk190 = temp.cameraPositionOrJitter;
                pView->unk198 = temp.frameNumber;
            }
        } else {
            CameraBlockSnapshot past{};
            past.cameraMatrix = pView->unk150;
            past.cameraPositionOrJitter = pView->unk190;
            past.frameNumber = pView->unk198;
            pastProjections[key] = past;
        }
    }
    return result;
}


uintptr_t CreationEngineRendererModule::onTaaPass(RE::CreationRendererPrivate::RenderPass* pPass, RE::CreationRendererPrivate::RenderGraphData* pRenderGraphData, RE::CreationRendererPrivate::RenderPassData* renderPassData)
{
    auto instance    = Get();
    auto original_fn = instance->taa_vfunc7_hook->get_original<decltype(CreationEngineRendererModule::onTaaPass)>();
    auto result = original_fn(pPass, pRenderGraphData, renderPassData);
    auto fc       = CreationEngineRendererModule::GetGlobalFrameCount();
    auto context     = reinterpret_cast<RE::RenderGraphDataD3D12Context*>(pRenderGraphData->getCommandList());
    auto commandList = context->pID3D12CommandList;
    auto vr = VR::get();

    if(vr->is_hmd_active() && Constants::enabledResourcesToCopy[2]) {
        auto resource = pRenderGraphData->getResourceByIndex(2, (fc - 1) & 1);
        instance->SwapBuffer(commandList, resource, 2,  (fc - 1) & 1, fc & 1);
    }
    if(vr->is_hmd_active() && Constants::enabledResourcesToCopy[3]) {
        auto resource = pRenderGraphData->getResourceByIndex(3, (fc - 1) & 1);
        instance->SwapBuffer(commandList, resource, 3,  (fc - 1) & 1, fc & 1);
    }
    return result;
}