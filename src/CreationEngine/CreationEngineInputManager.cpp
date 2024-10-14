//
// Created by sergp on 6/13/2024.
//
#include <memory>
#include <cstdint>

#include "CreationEngineInputManager.h"
#include <CreationEngine/memory/offsets.h>
#include <REL/Relocation.h>
#include <mods/VR.hpp>

CreationEngineInputManager::~CreationEngineInputManager() {
    vigem_target_remove(client, pad);
    vigem_target_free(pad);
}

void CreationEngineInputManager::Reset() {
    if(!connected) {
        return;
    }
    vigem_target_remove(client, pad);
    vigem_target_free(pad);
    connected = false;
    Init();
}

void CreationEngineInputManager::Init() {
    if(connected || client == nullptr) {
        return;
    }
    const auto retval = vigem_connect(client);

    if (!VIGEM_SUCCESS(retval))
    {
        spdlog::error("ViGEm Bus connection failed with error code: 0x %d", retval);
        return;
    }
    //
    // Add client to the bus, this equals a plug-in event
    //
    pad = vigem_target_x360_alloc();

    const auto pir = vigem_target_add(client, pad);

    if (!VIGEM_SUCCESS(pir))
    {
        spdlog::error("Target plugin failed with error code: 0x %d", pir);
    }
    spdlog::info("ViGEm Bus connection successful!");
    connected = true;

}

bool CreationEngineInputManager::Hook() {
    spdlog::info("Entering CreationEngineInputManager::Hook().");
    REL::Relocation<uintptr_t> gamepadDevicePollFuncAddr{ GameStore::MemoryOffsets::Gamepad::PollVfunc() };
    m_poll_events_hook = std::make_unique<FunctionHook>(gamepadDevicePollFuncAddr.address(), reinterpret_cast<uintptr_t>(&CreationEngineInputManager::onPollGamepadState));
    if(!m_poll_events_hook->create()) {
        spdlog::error("Failed to hook gamepadDevicePoll");
        return false;
    }
    return true;
}

CreationEngineInputManager::CreationEngineInputManager(): connected(false), client(vigem_alloc()), pad(nullptr)
{
    if (client == nullptr) {
        spdlog::error("Uh, not enough memory to do that?!");
        return;
    }
    Hook();
}

void CreationEngineInputManager::onPollGamepadState(__int64 a1, double xmm2) {
    static auto instance = CreationEngineInputManager::Get();
    instance->UpdateDeviceState();
    static auto original_fn = instance->m_poll_events_hook->get_original<decltype(CreationEngineInputManager::onPollGamepadState)>();
    return original_fn(a1, xmm2);
}

void CreationEngineInputManager::UpdateDeviceState() {
    if(!connected) {
        return;
    }
    XUSB_REPORT report = {0};
    static auto vr = VR::get();

    if (!vr->is_hmd_active()) {
        return;
    }
//    spdlog::info("Updating input state fc[{}]", vr->m_frame_count);

    vr->on_xinput_get_state(&report);
    vigem_target_x360_update(client, pad, report);
}
