#include "CreationEngineSingletonManager.h"
#include <CreationEngine/RE2/offsets.h>
#include <REL/Relocation.h>

RE::Main::SceneGraphRoot* CreationEngineSingletonManager::GetSceneGraphRoot()
{
    static REL::Relocation<RE::Main::SceneGraphRoot**> singleton{RE2::MemoryOffsets::GlobalSceneGraphRoot()};
    return *singleton;
}

RE::StorageTable::RenderGraphHandle* CreationEngineSingletonManager::GetRenderGraphHandle()
{
    static REL::Relocation<RE::StorageTable::RenderGraphHandle**> singleton{RE2::MemoryOffsets::StorageTable::RenderGraphHandle::Singleton()};
    return *singleton;
}

RE::PlayerCamera* CreationEngineSingletonManager::GetPlayerCameraSingleton()
{
    static REL::Relocation<RE::PlayerCamera**> singleton{RE2::MemoryOffsets::PlayerCamera::Singleton()};
    return *singleton;
}

RE::TESObjectREFR* CreationEngineSingletonManager::GetPlayerRef()
{
    static REL::Relocation<RE::TESObjectREFR**> singleton{RE2::MemoryOffsets::GlobalPlayerRef()};
    return *singleton;
}
