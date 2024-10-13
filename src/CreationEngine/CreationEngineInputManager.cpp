//
// Created by sergp on 6/13/2024.
//
#include <memory>
#include <cstdint>

#include "CreationEngineInputManager.h"
#include <CreationEngine/memory/offsets.h>
#include <REL/Relocation.h>
#include <mods/VR.hpp>
#include <polyhook2/Detour/x64Detour.hpp>

std::unique_ptr<PLH::x64Detour> gamepadDevicePollHook = nullptr;
typedef void (*GamepadDevicePollDetour)(__int64 a1, double xmm2);
GamepadDevicePollDetour originalGamepadDevicePoll = nullptr;

void gamepadDevicePollDetour(__int64 a1, double xmm2) {
    CreationEngineInputManager::Get()->UpdateDeviceState();
    return originalGamepadDevicePoll(a1, xmm2);
}


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

//    REL::Relocation<uintptr_t> gamepadDevicePollFuncAddr{ REL::ID(179249 ) };
    REL::Relocation<uintptr_t> gamepadDevicePollFuncAddr{ GameStore::MemoryOffsets::Gamepad::PollVfunc() };
    gamepadDevicePollHook = std::make_unique<PLH::x64Detour>(static_cast<uint64_t>(gamepadDevicePollFuncAddr.address()), reinterpret_cast<uint64_t>(&gamepadDevicePollDetour), reinterpret_cast<uint64_t*>(&originalGamepadDevicePoll));
    gamepadDevicePollHook->hook();
    return true;
}

CreationEngineInputManager::CreationEngineInputManager(): connected(false), client(vigem_alloc()), pad(nullptr)
{
    if (client == nullptr) {
        spdlog::error("Uh, not enough memory to do that?!");
        return;
    }
    CreationEngineInputManager::Hook();
}

void CreationEngineInputManager::UpdateDeviceState() {
    if(!connected) {
        return;
    }
    XUSB_REPORT report = {0};
    auto vr = VR::get();

    if (!vr->is_hmd_active()) {
        return;
    }
//    spdlog::info("Updating input state fc[{}]", vr->m_frame_count);

    vr->on_xinput_get_state(&report);
    vigem_target_x360_update(client, pad, report);
}
