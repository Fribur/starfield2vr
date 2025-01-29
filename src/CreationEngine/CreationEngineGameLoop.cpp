#include "CreationEngineGameLoop.h"
#include "CreationEngineCameraManager.h"
#include "CreationEngineRendererModule.h"
#include "CreationEngineSingletonManager.h"
#include <CreationEngine/models/ModSettingsStore.h>
#include <SFVR.hpp>
#include <mods/VR.hpp>
#include <safetyhook/easy.hpp>
/*
void logWithExtraData(std::string_view message) {
    spdlog::info("[{:X}:{}] frameCount[m={},r={}]", GetCurrentThreadId(), message.data(), GameFlow::gameLoopFrameCount(), GameFlow::renderLoopFrameCount());
}*/

/*
// Compare RCX against this value
static std::uintptr_t gCheckValue = 0xFFFFFFFFFF;
// Store the caller's return address for later inspection
static std::uintptr_t gCallerAddr = 0;
static std::uintptr_t* pPcallerAddr = &gCallerAddr;

#pragma optimize("", off)
__attribute__((naked))
extern "C" void onCopyMatrix()
{
    __asm {
        push rax                 // Preserve RAX
        push r9

        mov rax, gCheckValue
        cmp rcx, rax
        jne skip_logic
        mov r9, pPcallerAddr
        mov rax, [rsp + 0x10]
        mov [r9], rax

    skip_logic:
        pop r9
        pop rax                  // Get the return address

        // Original matrix copy logic
        mov rax, rcx
        sub rdx, rcx
        mov r8d, 3
    loop_start:
        vmovups xmm0, xmmword ptr [rdx + rax]
        vmovups xmmword ptr [rax], xmm0
        lea rax, [rax + 0x10]
        sub r8, 1
        jnz loop_start
        mov rax, rcx
        ret
    }
}
#pragma optimize("", on)*/

void CreationEngineGameLoop::install_hooks()
{
    // 0x25dafe4 - this one works but sometimes give 2 ticks
//    REL::Relocation<uintptr_t> onWorldTickFn{ (uintptr_t )MemoryScan::mod + 0x25dafe4 };
//    m_worldTick_hook = safetyhook::create_inline((void*)onWorldTickFn.address(), (void*)worldTick);
//    //        std::make_unique<FunctionHook>(onWorldTickFn.address(), reinterpret_cast<uint64_t>(&worldTick));
//    if(!m_worldTick_hook) {
//        spdlog::error("Failed to create hook for onWorldTick");
//    }
/*

    REL::Relocation<uintptr_t> setReflexMarkerInternalFn{ (uintptr_t )MemoryScan::mod + 0x3582490 };
    m_setReflexMarkerInternalHook = safetyhook::create_inline((void*)setReflexMarkerInternalFn.address(), (void*)setReflexMarkerInternal);
    if(!m_setReflexMarkerInternalHook) {
        spdlog::error("Failed to create hook for setReflexMarkerInternal");
    }
*/

//    REL::Relocation<uintptr_t> onCopyMatrixFn{ (uintptr_t )MemoryScan::mod + 0x12745f8  };
//    m_onCopyMatrixHook = std::make_unique<FunctionHook>(onCopyMatrixFn.address(), reinterpret_cast<uintptr_t >(&onCopyMatrix));
//    if(!m_onCopyMatrixHook->create()) {
//        spdlog::error("Failed to create hook for onCopyMatrix");
//    }

}


uintptr_t CreationEngineGameLoop::worldTick(uintptr_t rcx)
{
    static auto instance = Get();
//    logWithExtraData("worldTick");
    static bool dynamic_reflex_set_marker = false;
    if (!dynamic_reflex_set_marker) {
        REL::Relocation<uintptr_t*> onSlReflexSetMarkerFn{ (uintptr_t )MemoryScan::mod + 0x6a50368  };
        if (*onSlReflexSetMarkerFn.get() != 0) {
            instance->m_onSlReflexSetMarkerHook = std::make_unique<PointerHook>((void**)onSlReflexSetMarkerFn.get(), (void*)&CreationEngineGameLoop::onSlReflexSetMarker);
            spdlog::info("Hooked m_onSlReflexSetMarkerHook");
            dynamic_reflex_set_marker = true;
        }
        else {
            spdlog::error("m_onSlReflexSetMarkerHook not yet initialized");
        }
    }
    auto result =  instance->m_worldTick_hook.call<uintptr_t>(rcx);
    return result;
}

sl::Result CreationEngineGameLoop::onSlReflexSetMarker(sl::ReflexMarker marker, const sl::FrameToken& frame)
{
    static auto pModule = CreationEngineGameLoop::Get();
    static auto     original_fn      = pModule->m_onSlReflexSetMarkerHook->get_original<decltype(CreationEngineGameLoop::onSlReflexSetMarker)*>();
    int marker_enum = marker;
    uint32_t frame_n = frame;
//    logWithExtraData(std::format("onSlReflexSetMarker[{}, f={}]", marker_enum, frame_n));
    return original_fn(marker, frame);
}
/*

uintptr_t CreationEngineGameLoop::setReflexMarkerInternal(uintptr_t rcx, uint32_t marker, uint32_t oldFrameIndex)
{
//    logWithExtraData(std::format("setReflexMarkerInternal[{}, f={}]", marker, oldFrameIndex));
    static auto instance = Get();
    static bool engine_notified = false;
    static     auto        vr            = VR::get();
    static auto cameraModule = CreationEngineCameraManager::Get();
    static int frame_sync = 0;
    if ((marker == 6 || marker == 0 || marker == 1) && !engine_notified) {
        CreationEngineRendererModule::Get()->SetWindowSize(0,0);
        g_framework->run_imgui_frame(false);
        vr->on_pre_begin_rendering(nullptr);
        vr->on_begin_rendering(nullptr);
        frame_sync--;
        cameraModule->UpdateWorldCamera();
        engine_notified = true;
    }
    if(marker == 3) {
        frame_sync++;
        if(vr->get_runtime()->loaded && vr->m_frame_count % 2 == 0 && frame_sync > 0) {
            spdlog::info("Detected frame inconsistency, resetting frame sync");
            vr->m_skip_frames = 1;
        }
    }

    // Reset notification if marker is 1
    if (marker == 1) {
        engine_notified = false;
    }
    return instance->m_setReflexMarkerInternalHook.call<uintptr_t>(rcx, marker, oldFrameIndex);
}
*/
