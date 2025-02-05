//
// Created by sergp on 1/9/2025.
//

#include "HavokModule.h"
#include "CreationEngineSingletonManager.h"
#include <CreationEngine/memory/ScanHelper.h>
#include <CreationEngine/models/GameFlow.h>
#include <CreationEngine/models/ModSettingsStore.h>
#include "RE/N/NiCamera.h"

HavokModule::HavokModule() {
    install_hooks();
}

void HavokModule::install_hooks() {
    /*REL::Relocation<uintptr_t> onUpdateConstantBufferViewAddr{(uintptr_t)MemoryScan::mod + 0x7993C0};
    ni_pointer_set_hook = std::make_unique<FunctionHook>(onUpdateConstantBufferViewAddr.address(), reinterpret_cast<uintptr_t>(&onNiPointerSet));
    if(!ni_pointer_set_hook->create()) {
        spdlog::error("Failed to hook NiPointerSet");
    }*/
/*
    REL::Relocation<uintptr_t> onHavokRayCastFromViewMuzzleAddr{(uintptr_t)MemoryScan::mod + 0x1a276ac};
    m_havok_raycast_weapon_hook = std::make_unique<FunctionHook>(onHavokRayCastFromViewMuzzleAddr.address(), reinterpret_cast<uintptr_t>(&onHavokRaycast));
    if(!m_havok_raycast_weapon_hook->create()) {
        spdlog::error("Failed to hook Havok Raycast");
    }*/

    /*REL::Relocation<uintptr_t> onHavokRayCastFromWeaponMuzzleAddr{(uintptr_t)MemoryScan::mod + 0x2fdfdbc};
    m_havok_raycast_weapon2_hook = std::make_unique<FunctionHook>(onHavokRayCastFromWeaponMuzzleAddr.address(), reinterpret_cast<uintptr_t>(&onHavokRaycast2));
    if(!m_havok_raycast_weapon2_hook->create()) {
        spdlog::error("Failed to hook Havok Raycast 2");
    }*/

}

uintptr_t HavokModule::onNiPointerSet(uintptr_t a_pointer, uintptr_t a_value)
{
    static auto instance = Get();
    auto original_fn = instance->ni_pointer_set_hook->get_original<decltype(onNiPointerSet)>();
    auto player = CreationEngineSingletonManager::GetPlayerRef();
    auto collision = (uintptr_t )&player->crosshairHit;
    if(collision == a_pointer) {
        spdlog::info("Collision detected");
    }
    return original_fn(a_pointer, a_value);

}

RE::NiAVObject* HavokModule::onHavokRaycast(uintptr_t* testObjectCEll, CEDebug::RayCastQuery* rayCastQuery)
{
    static auto instance = Get();
    auto original_fn = instance->m_havok_raycast_weapon_hook->get_original<decltype(onHavokRaycast)>();
    auto result = original_fn(testObjectCEll, rayCastQuery);
    if(result) {
//          sdk::g_modSettings.debugData.nodes.push_back(result);
    }
    return result;
}

char HavokModule::onHavokRaycast2(uintptr_t* source, CEDebug::RayCastQuery* rayCastQuery)
{
    static auto instance = Get();
    auto original_fn = instance->m_havok_raycast_weapon2_hook->get_original<decltype(onHavokRaycast2)>();
    auto result = original_fn(source, rayCastQuery);
    if(result) {
        auto size = GameFlow::gStore.debugData.points.size();
        RE::NiCamera* wCamera = CreationEngineSingletonManager::GetSceneGraphRoot()->worldCamera;
        auto player = CreationEngineSingletonManager::GetPlayerRef();
        const auto hit = *(RE::NiPoint3*) &rayCastQuery->point70;
//        if(size == 2 && GetAsyncKeyState(VK_F12) & 1) {
//            spdlog::info("Break here");
//        }
        static int frames = 0;
        if(player && GameFlow::isAimingDownSights() && size > 0) {
            auto wts = wCamera->WorldToScreen(hit);
            if(frames > 210 && fabs(wts.x) < .1f && fabs(wts.y) < .1f) {
                spdlog::info("Hit detected {} size {}", fmt::ptr(source), size);
            }
            frames++;
        }
        else {
            frames = 0;
        }


//        // origin
//        GameFlow::gStore.debugData.points.push_back({rayCastQuery->point30});
//        sdk::gSettings.debugData.points.push_back({rayCastQuery->point40});
//        // target
        GameFlow::gStore.debugData.points.push_back({rayCastQuery->point70});
    }
    return result;
}
